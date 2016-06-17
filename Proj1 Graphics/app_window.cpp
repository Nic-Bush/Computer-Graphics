
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

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
   _gouradshader_tex.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_tex_gourad.glsl");
   _fragsh2_tex.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_tex_gourad.glsl");
   
   
   _prog.init_and_link ( _vertexsh, _fragsh);
   _prog2.init_and_link(_gouradshader, _fragsh2);
   _prog3.init_and_link(_gouradshader_tex, _fragsh2_tex);
   // Init my scene objects:
   _axis.init ( _prog );
   _wheel1.init(_prog2);
   _body.init(_prog2);
   _wheel2.init(_prog2);
   _tail.init(_prog2);
   _head.init(_prog2);
   _floor.init(_prog3);
   nohand.init(_prog);
   nolower.init(_prog);
   noupper.init(_prog);
   _capsule.init(_prog2);

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
   {
   case ' ': space = !space; cam = !cam; if (!cam) { _rotx = 0; _roty = 0; }

			 
			 redraw(); break;
	  case 27 : exit(1); // Esc was pressed
	  case 'x':normtype = 0; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); redraw(); break;
	  case 'z':glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); redraw(); break;
	  case 'w':zmove = zmove + .05; redraw(); break;
	  case 'a': xmove = xmove + .05; redraw(); break;
	  case 's':zmove = zmove - .05; redraw(); break;
	  case 'd': xmove = xmove - .05; redraw(); break;
	  case 'e':  ymove = ymove + .05;  redraw(); break;
	  case 'q': if (ymove>0) { ymove = ymove - .05; } redraw(); break;
	  case 't': imroty = imroty + .2; redraw(); break;
	  case 'r': imroty = imroty - .2; redraw(); break;
	  case 'f': armrotx = armrotx - .2; redraw(); break;
	  case 'g': armrotx = armrotx + .2; redraw(); break;
	  case'y': scalex = scalex - .05; redraw(); break;
	  case'u': scalex = scalex + .05; redraw(); break;
	  case'h': scaley = scaley - .05; redraw(); break;
	  case'j': scaley = scaley + .05; redraw(); break;
	  case'i': scalez = scalez - .05; redraw(); break;
	  case'o': scalez = scalez + .05; redraw(); break;
	  case 'k': if (leg > -3.14/16) { leg = leg - .05; } redraw(); break;
	  case 'l': if (leg < 3.14/16) { leg = leg + .05; } redraw(); break;
	  case 'c':if (foot > -3.14 / 4) { foot = foot - .05; } redraw(); break;
	  case 'v':if (foot < 3.14/4 ) { foot = foot + .05; }  redraw(); break;
	  case 'b':if (footy > -3.14 / 4) { footy = footy - .05; }redraw(); break;
	  case 'n':if (footy < 3.14 / 4) { footy = footy + .05; }redraw(); break;
   }
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
   {
   case GLUT_KEY_LEFT:      if (cam) { _roty -= incr; }
							else { _roty = 0; }
							break;
      case GLUT_KEY_RIGHT:     if (cam) { _roty += incr; }
							   else { _roty = 0; }
							   break;
	  case GLUT_KEY_UP:        if (cam) { _rotx -= incr; }
							   else { _rotx = 0; }; break;
      case GLUT_KEY_DOWN:      if (cam) { _rotx += incr; }
							   else { _rotx = 0 ; }; break;
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
	   _wheel1.buildwheel(nfaces, normtype);

   //if(_model2.changed)
	   _wheel2.buildwheel(nfaces, normtype);

   //if(_model3.changed)
	   _body.buildBody(nfaces, normtype);
   
	   _tail.buildBody(nfaces, normtype);

	   _head.buildwheel(nfaces, normtype);
   
	   _floor.buildfloor(nfaces, normtype);

	   _capsule.build(10,0);

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
   GsMat rx, ry, stransf, stransf1, stransf2, wheel1transf,wheelrot, wheel2transf,moved,wheel1scale,tailrot,tailtrans,headtrans,headrot,headscale, imrot,im2pos,im3pos,im4pos,im5pos,handscale,regchicken1,regchicken2,ytransf,leg1trans,leg2trans,finger1,fingerrot,finger2,legrot,footrot,footydir;
   footydir.rotz(footy);

   legrot.rotx((abs(leg + xmove + ymove + zmove)- abs(int(leg + xmove + ymove + zmove)) -.5) *.5 );
   footrot.rotx(foot);
   fingerrot.rotx(3.14/2);
   finger1.translation(.5,-1.4,.6);
   finger2.translation(-.5, -1.4, .6);
   leg1trans.translation(.5,-1,.5);
   leg2trans.translation(-.5, -1, .5);
   handscale.scale(scalex,scaley,scalez);
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   im2pos.translation(15,1,-10);
   ytransf.translation(0,.1,.1);
   im3pos.translation(15, 1, 10);
   im4pos.translation(-15, 1, -10);
   im5pos.translation(-15, 1, 10);
   imrot.roty(imroty);
   wheelrot.rotx(armrotx);
   headscale.scale(.75);
   headtrans.translation(0,1.5,.75);
   tailtrans.translation(0,1,-1);
   tailrot.rotx(-3.14579/4);
   wheel1scale.scale(.5);
   moved.translation(xmove,ymove+1.65,zmove);
   regchicken1.translation(1 +.25*scalex, 0, .5);
   regchicken2.translation(-1 - .25*scalex, 0, .5);
   wheel1transf.translation(1.25,0,.5);
   wheel2transf.translation(-1.25,0,.5);
   
   stransf = rx*ry; // set the scene transformation matrix //manipulate arm


	   GsVec eye(0, 0, 60), center(0, 0, 0), up(0, 1, 0);
	   camview.lookat(eye, center, up); // set our 4x4 "camera" matrix
 
   float aspect=1.0f, znear=0.1f, zfar=100.0f;
  
	   persp.perspective(_fovy, aspect, znear, zfar); // set our 4x4 perspective matrix
   
	   if (space == 1) {
		   sproj = persp * camview; // set final scene projection
	   }
	   else {
		   GsMat rot, movement;
		   movement.translation(-xmove,-ymove,-zmove);
		   rot.roty(-imroty);
		   sproj = persp * camview*rot*movement; // set final scene projection
	   }
   // Draw:
	   _capsule.draw(stransf*moved*imrot*leg1trans*wheel1scale*legrot , sproj, l, m);
	   _capsule.draw(stransf*moved*imrot*leg2trans*wheel1scale*legrot, sproj, l, m);
	   _capsule.draw(stransf*moved*imrot*legrot*finger1*wheel1scale*fingerrot*footrot*footydir, sproj, l, m);
	   _capsule.draw(stransf*moved*imrot*legrot*finger2*wheel1scale*fingerrot*footrot*footydir, sproj, l, m);

	   _capsule.draw(stransf*ytransf*shadowMatrix()*moved*imrot*leg1trans*wheel1scale*legrot, sproj, l, m5);
	   _capsule.draw(stransf*ytransf*shadowMatrix()*moved*imrot*leg2trans*wheel1scale*legrot, sproj, l, m5);
	   _capsule.draw(stransf*ytransf*shadowMatrix()*moved*imrot*legrot*finger1*wheel1scale*fingerrot*footrot*footydir, sproj, l, m5);
	   _capsule.draw(stransf*ytransf*shadowMatrix()*moved*imrot*legrot*finger2*wheel1scale*fingerrot*footrot*footydir, sproj, l, m5);

	   if (_viewaxis) _floor.draw(stransf, sproj, l, m4);
	   if (_viewaxis) _head.draw(stransf*moved*imrot*headtrans*headscale, sproj, l, m);
	   if (_viewaxis) _tail.draw(stransf*moved*imrot*tailtrans*wheel1scale*tailrot, sproj, l, m);
	   if (_viewaxis) _wheel1.draw(stransf*moved*imrot*regchicken1*wheel1scale*handscale*wheelrot, sproj, l, m);
	   if (_viewaxis) _wheel2.draw(stransf*moved*imrot*regchicken2*wheel1scale*handscale*wheelrot, sproj, l, m2);
	   if (_viewaxis) _body.draw(stransf*moved*imrot, sproj, l, m3);

	   if (_viewaxis) _head.draw(stransf*ytransf*shadowMatrix()*moved*imrot*headtrans*headscale, sproj, l, m5);
	   if (_viewaxis) _tail.draw(stransf*ytransf*shadowMatrix()*moved*imrot*tailtrans*wheel1scale*tailrot, sproj, l, m5);
	   if (_viewaxis) _wheel1.draw(stransf*ytransf*shadowMatrix()*moved*imrot*regchicken1*wheel1scale*handscale*wheelrot, sproj, l, m5);
	   if (_viewaxis) _wheel2.draw(stransf*ytransf*shadowMatrix()*moved*imrot*regchicken2*wheel1scale*handscale*wheelrot, sproj, l, m5);
	   if (_viewaxis) _body.draw(stransf*ytransf*shadowMatrix()*moved*imrot, sproj, l, m5);

	   if (_viewaxis) _head.draw(stransf*im2pos*headtrans*headscale, sproj, l, m);
	   if (_viewaxis) _tail.draw(stransf*im2pos*tailtrans*wheel1scale*tailrot, sproj, l, m);
	   if (_viewaxis) _wheel1.draw(stransf*im2pos*wheel1transf*wheel1scale*wheelrot, sproj, l, m);
	   if (_viewaxis) _wheel2.draw(stransf*im2pos*wheel2transf*wheel1scale*wheelrot, sproj, l, m2);
	   if (_viewaxis) _body.draw(stransf*im2pos, sproj, l, m3);
	   if (_viewaxis) _axis.draw(stransf, sproj);

	   if (_viewaxis) _head.draw(stransf*im3pos*headtrans*headscale, sproj, l, m);
	   if (_viewaxis) _tail.draw(stransf*im3pos*tailtrans*wheel1scale*tailrot, sproj, l, m);
	   if (_viewaxis) _wheel1.draw(stransf*im3pos*wheel1transf*wheel1scale*wheelrot, sproj, l, m);
	   if (_viewaxis) _wheel2.draw(stransf*im3pos*wheel2transf*wheel1scale*wheelrot, sproj, l, m2);
	   if (_viewaxis) _body.draw(stransf*im3pos, sproj, l, m3);
	   if (_viewaxis) _axis.draw(stransf, sproj);

	   if (_viewaxis) _head.draw(stransf*im4pos*headtrans*headscale, sproj, l, m);
	   if (_viewaxis) _tail.draw(stransf*im4pos*tailtrans*wheel1scale*tailrot, sproj, l, m);
	   if (_viewaxis) _wheel1.draw(stransf*im4pos*wheel1transf*wheel1scale*wheelrot, sproj, l, m);
	   if (_viewaxis) _wheel2.draw(stransf*im4pos*wheel2transf*wheel1scale*wheelrot, sproj, l, m2);
	   if (_viewaxis) _body.draw(stransf*im4pos, sproj, l, m3);
	   if (_viewaxis) _axis.draw(stransf, sproj);

	   if (_viewaxis) _head.draw(stransf*im5pos*headtrans*headscale, sproj, l, m);
	   if (_viewaxis) _tail.draw(stransf*im5pos*tailtrans*wheel1scale*tailrot, sproj, l, m);
	   if (_viewaxis) _wheel1.draw(stransf*im5pos*wheel1transf*wheel1scale*wheelrot, sproj, l, m);
	   if (_viewaxis) _wheel2.draw(stransf*im5pos*wheel2transf*wheel1scale*wheelrot, sproj, l, m2);
	   if (_viewaxis) _body.draw(stransf*im5pos, sproj, l, m3);
	   if (_viewaxis) _axis.draw(stransf, sproj);
   
   
   if (normson) {
	   //nohand.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow*handtranslation*rotationarm, sproj);
	   //nolower.draw(stransf*endtransf*rotationshoulder*upperarmtranslation*rotationelbow, sproj);
	   //noupper.draw(stransf*endtransf*lowerarmtranslation*rotationshoulder, sproj);
   }
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

