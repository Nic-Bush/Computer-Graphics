
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

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

   // Init my scene objects:
   _axis.init ( _prog );
   _clock.init(_prog);
   xShadow = 1.0;
   yShadow = 1.0;
   zShadow = 1.0;
 }

GsMat AppWindow::shadowMatrix() {
	float ground[4] = { 0,0.5,0,0 };
	float light[4] = { xShadow,yShadow,zShadow,0 };

	float dot = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];

	GsMat shadowMat(dot - ground[0] * light[0],
		0 - ground[1] * light[0],
		0 - ground[2] * light[0],
		0 - ground[3] * light[0],
		0 - ground[0] * light[1],
		dot - ground[1] * light[1],
		0 - ground[2] * light[1],
		0 - ground[3] * light[1],
		0 - ground[0] * light[2],
		0 - ground[1] * light[2],
		dot - ground[2] * light[2],
		0 - ground[3] * light[2],
		0 - ground[0] * light[3],
		0 - ground[1] * light[3],
		0 - ground[2] * light[3],
		dot - ground[3] * light[3]);

	return shadowMat;
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
void AppWindow::glutKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': _viewaxis = !_viewaxis; 
		startTime = glutGet(GLUT_ELAPSED_TIME);
		CLOCK_RUN = !CLOCK_RUN;
		pausedMinutes = minutes;
		pausedSeconds = seconds;
		redraw(); break;
	case 'q': xShadow += .01; redraw(); break;
	case 'a': xShadow -= .01; redraw(); break;
	case 'w': yShadow += .01; redraw(); break;
	case 's': yShadow -= .01; redraw(); break;
	case 'e': zShadow += .01; redraw(); break;
	case 'd': zShadow -= .01; redraw(); break;
	case '/': showshadow = !showshadow; break;
	case 13: startTime = glutGet(GLUT_ELAPSED_TIME); pausedMinutes = 0.0; pausedSeconds = 0.0;
		minutes = 0.0; seconds = 0.0; redraw(); break;
	case 27: exit(1); // Esc was pressed
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
   _clock.build((float)(glutGet(GLUT_ELAPSED_TIME) / 60.0));
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

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   _clock.draw(stransf, sproj,minutes,seconds);
   if (showshadow) {
	   _clock.draw(stransf*shadowMatrix(), sproj, minutes, seconds);
   }
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::glutIdle() {
	if (CLOCK_RUN) {
		int deltaTime = glutGet(GLUT_ELAPSED_TIME) - startTime;
		minutes = pausedMinutes + (double)(((60*deltaTime / 1000) / -60) * 2 * M_PI / 60);
		seconds = pausedSeconds + (double)(((60*deltaTime / 1000) / -1) * 2 * M_PI / 60);
		redraw();
	}
}