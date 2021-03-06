
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"


float len = 1.0;
float rt = 1.0;
float rb = 1.0;
int degree = 0;
int type = 0;
bool wire = false;
float theta = 0;

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
 }

void AppWindow::initPrograms ()
 {
   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../vsh_mcol_flat.glsl" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../fsh_flat.glsl" );
   _prog.init_and_link ( _vertexsh, _fragsh );
   _gouradshader.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_gourad.glsl");
   _fragsh2.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_gourad.glsl");
   _prog2.init_and_link(_gouradshader, _fragsh2);
   // Init my scene objects:
   _axis.init ( _prog );
   _capsule.init(_prog);
   _capsule_2.init(_prog2);
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
   switch ( key )
    { case ' ': _viewaxis = !_viewaxis; redraw(); break;
	  case 27 : exit(1); // Esc was pressed

	  case 'q': degree++; redraw(); break;
	  case 'w': rt += 0.01f; redraw(); break;
	  case 's': rt -= 0.01f; redraw(); break;
	  case 'e': rb += 0.01f; redraw(); break;
	  case 'd': rb -= 0.01f; redraw(); break;
	  case 'v': type = 1; redraw(); break;
	  case 'x': type = 0; redraw(); break;
	  case 'z': wire = !wire; if (wire == true) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
				else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } redraw(); break;
	}
   if (degree >= 1 && key == 'a')
   {
degree--; 
redraw(); 
   }
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:      _roty-=incr; break;
      case GLUT_KEY_RIGHT:     _roty+=incr; break;
      case GLUT_KEY_UP:        _rotx-=incr; break;
      case GLUT_KEY_DOWN:      _rotx+=incr; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      case GLUT_KEY_HOME:      _fovy=GS_TORAD(60.0f); _rotx=_roty=0; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }
   if (type == 1) {
	   _capsule.build(len, rt, rb, degree);
   }
   else {
	   _capsule_2.build(len, rt, rb, degree);
   }
   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   GsVec eye(0,0,2), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.
   GsMat trans;
   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   if (type == 1)
   {
	  // for (int j = -1; j += 1; j <= 1)
	   //{
		 //  for (int i = -2; i += 1; i <= 2)
		   //{
			   //trans.translation(double(j),double(i),double(sin(theta + .3 * i + .2 * j)));
			   _capsule.draw(stransf, sproj);
		   //}
	   //}
   }
   else {
	   _capsule_2.draw(stransf, sproj, l, m);
   }
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

