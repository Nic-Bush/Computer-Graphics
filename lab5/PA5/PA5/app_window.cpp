
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
   _viewmodel1 = true;
   _viewmodel2 = true;
   _viewmodel3 = true;
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
   _gouradshader.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_gourad.glsl");
   _fragsh2.load_and_compile(GL_FRAGMENT_SHADER,"../fsh_gourad.glsl");
   _prog.init_and_link ( _vertexsh, _fragsh);
   _prog2.init_and_link(_gouradshader, _fragsh2);
   // Init my scene objects:
   _axis.init ( _prog );
   _model1.init(_prog2);
   _model2.init(_prog2);
   _model3.init(_prog2);
   nohand.init(_prog);
   nolower.init(_prog);
   noupper.init(_prog);
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
    { case ' ': _viewaxis != _viewaxis; redraw(); break;
	  case 27 : exit(1); // Esc was pressed
	  case 'c':normtype = 1; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); redraw(); break;
	  case'v':normson = !normson; redraw();break;
	  case 'a': shoulder= shoulder +.05; redraw(); break;
	  case 'q': shoulder = shoulder - .05; redraw(); break;
	  case 'x':normtype = 0; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); redraw(); break;
	  case 'z':glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); redraw(); break;
	  case 's':elbow = elbow + .05;redraw() ; break;
	  case 'w':elbow = elbow - .05; redraw(); break;
	  case 'd': hand = hand + .05; redraw(); break;
	  case 'e': hand = hand - .05; redraw(); break;
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

void AppWindow::glutMouse ( int b, int s, int x, int y)
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
    _axis.build(20.0); // axis has radius 1.0


   
   //if (_model1.changed) // needs update
	   _model1.buildhand(nfaces, normtype);

   //if(_model2.changed)
	   _model2.buildupperarm(nfaces, normtype);

   //if(_model3.changed)
	   _model3.buildlowerarm(nfaces, normtype);
   
   if (nohand.changed) // needs update
   {
	   nohand.buildhand(nfaces, normtype);

   }
   if (nolower.changed)
   {
	   nolower.buildlowerarm(nfaces, normtype);
   }

   if (noupper.changed)
   {
	   noupper.buildupperarm(nfaces, normtype);
   }
   //scale



   // Define our scene transformation:
   GsMat rx, ry, stransf, stransf1, stransf2, handtranslation, lowerarmtranslation, upperarmtranslation, rotationarm, rotationelbow, rotationshoulder, endtransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   
  
   handtranslation.translation(0, 0, 12);
   lowerarmtranslation.translation(0, 0, 0);
   upperarmtranslation.translation(0, 0, 13);
   endtransf.translation(0,0,-16);
   rotationarm.rotx(hand);
   rotationelbow.rotx(elbow);
   rotationshoulder.rotx(shoulder);
   stransf = rx*ry; // set the scene transformation matrix //manipulate arm

   GsMat camview, persp, sproj;


   GsVec eye(0,0,60), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=100.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   sproj = persp * camview; // set final scene projection

   // Draw:
   if ( _viewaxis ) _model1.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow*handtranslation*rotationarm, sproj, l , m);
   if (_viewaxis) _model2.draw(stransf*endtransf*lowerarmtranslation*rotationshoulder, sproj, l, m2); 
   if (_viewaxis) _model3.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow, sproj, l, m3);
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   if (normson) {
	   nohand.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow*handtranslation*rotationarm, sproj);
	   nolower.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow, sproj);
	   noupper.draw(stransf*endtransf*lowerarmtranslation*rotationshoulder, sproj);
   }
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

