
# include <iostream>
# include "app_window.h"
#include<math.h>
AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _markc = GsColor::yellow;
   _w=w; _h=h;
 }
void AppWindow::glutIdle()
{
	_tris.changed = true;
	redraw();
}
void AppWindow::initPrograms ()
 {
   // We are following the OpenGL API version 4, 
   // see docs at: http://www.opengl.org/sdk/docs/man/

   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "vsh_mcol_flat.glsl" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "fsh_flat.glsl" );
   _prog.init_and_link ( _vertexsh, _fragsh );

   // Define buffers needed for each of your OpenGL objects:
   // (here they all use the same definitions because we are using the same shaders)
   // Program for rendering triangles:
   { GlObjects& o = _tris;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vProj" );
   }

   // Define buffers needed for each of your OpenGL objects:
   { GlObjects& o = _pts;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vProj" );
   }

   // Define buffers needed for each of your OpenGL objects:
   { GlObjects& o = _lines;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vProj" );
   }
 }


// mouse events are in window coordinates, but your scene is in [0,1]x[0,1],
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
    { case ' ': // space bar
	   std::cout << "Space pressed.\n";
       _mark.set ( 1.5, 1.5 );
       redraw();
	   break;

	  case 27: // Esc was pressed
	   exit(1);
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incx=0.02f;
   const float incy=0.02f;
   switch ( key )
    { case GLUT_KEY_LEFT:  _mark.x-=incx; break;
      case GLUT_KEY_RIGHT: _mark.x+=incx; break;
      case GLUT_KEY_UP:    _mark.y+=incy; break;
	  case GLUT_KEY_DOWN:  _mark.y -= incy; break;
      default: rd=false; // no redraw
	}

   if (rd) 
    { _lines.changed = true; // mark that new lines have to be generated
   _pts.changed = true;
      redraw(); // ask the window to be rendered when possible
    }
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   _markc = GsColor::yellow;
   _mark = windowToScene ( GsVec2(x,y) );
   _lines.changed = true; // mark that new lines have to be generated
   // note: a better design would be to only change the geometry when really needed so
   // that we minimize overhead with buffer definition and transfer, for example here
   // we could just send a transformation to our shader and keep the geometry the same.
   redraw();
 }

void AppWindow::glutMotion ( int x, int y )
 {
   _markc = GsColor::red;
   _mark = windowToScene ( GsVec2(x,y) );
   _lines.changed = true; // mark that new lines have to be generated
   redraw();
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
void AppWindow::buildObjects(float s)
{
	// Define a cross with some lines:
	if (_lines.changed) // needs update
	{
		_linecoords.clear(); _linecolors.clear();
		// Encode our lines in buffers according to _mark position, _markc color and size s:
		_linecoords.push_back(GsVec(-0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::red);
		_linecoords.push_back(GsVec(0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::red);
		_linecoords.push_back(GsVec(-0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::magenta);
		_linecoords.push_back(GsVec(0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::magenta);

		_linecoords.push_back(GsVec(-0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::blue);
		_linecoords.push_back(GsVec(-0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::blue);
		_linecoords.push_back(GsVec(0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::orange);
		_linecoords.push_back(GsVec(0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::orange);

		_linecoords.push_back(GsVec(-0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::gray);
		_linecoords.push_back(GsVec(0.0, 0.7, 0.0)); _linecolors.push_back(GsColor::gray);
		_linecoords.push_back(GsVec(0.0, 0.7, 0.0)); _linecolors.push_back(GsColor::green);
		_linecoords.push_back(GsVec(0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::green);

		_linecoords.push_back(GsVec(-0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::white);
		_linecoords.push_back(GsVec(0.0, -0.6, 0.0)); _linecolors.push_back(GsColor::white);
		_linecoords.push_back(GsVec(0.0, -0.6, 0.0)); _linecolors.push_back(GsColor::yellow);
		_linecoords.push_back(GsVec(0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::yellow);

		_linecoords.push_back(GsVec(-0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::blue);
		_linecoords.push_back(GsVec(-0.95, 0.0, 0.0)); _linecolors.push_back(GsColor::blue);
		_linecoords.push_back(GsVec(-0.95, -0.0, 0.0)); _linecolors.push_back(GsColor::red);
		_linecoords.push_back(GsVec(-0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::red);

		_linecoords.push_back(GsVec(0.9, -0.5, 0.0)); _linecolors.push_back(GsColor::magenta);
		_linecoords.push_back(GsVec(0.95, 0.0, 0.0)); _linecolors.push_back(GsColor::magenta);
		_linecoords.push_back(GsVec(0.95, -0.0, 0.0)); _linecolors.push_back(GsColor::green);
		_linecoords.push_back(GsVec(0.9, 0.6, 0.0)); _linecolors.push_back(GsColor::green);

		_linecoords.push_back(GsVec(_mark.x - s, _mark.y, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x + s, _mark.y, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x, _mark.y - s, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x, _mark.y + s, 0)); _linecolors.push_back(_markc);

		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _lines.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _linecoords.size() * 3 * sizeof(float), &_linecoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _lines.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _linecolors.size() * 4 * sizeof(gsbyte), &_linecolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_lines.changed = false;
	}

	// Define some white points:
	if (_pts.changed) // needs update
	{
		float a = 2;
		int b = 0;
		_ptcoords.clear(); _ptcolors.clear();
		// Encode some white points in buffers:
		_ptcoords.push_back(GsVec(_mark.x+0.5,_mark.y + 0.5, 0.0)); _ptcolors.push_back(GsColor::white);
		_ptcoords.push_back(GsVec(_mark.x-0.5,_mark.y -0.5, 0.0)); _ptcolors.push_back(GsColor::white);
		_ptcoords.push_back(GsVec(_mark.x + 0.5, _mark.y-0.5, 0.0)); _ptcolors.push_back(GsColor::white);
		_ptcoords.push_back(GsVec(_mark.x-0.5, _mark.y + 0.5, 0.0)); _ptcolors.push_back(GsColor::white);

		_ptcoords.push_back(GsVec(_mark.y + 0.25, _mark.x + 0.16, 0.0)); _ptcolors.push_back(GsColor::green);
		_ptcoords.push_back(GsVec(_mark.y-0.36,_mark.x -0.3, 0.0)); _ptcolors.push_back(GsColor::green);
		_ptcoords.push_back(GsVec(_mark.y + 0.52,_mark.x -0.87, 0.0)); _ptcolors.push_back(GsColor::green);
		_ptcoords.push_back(GsVec(_mark.y-0.91,_mark.x+ 0.21, 0.0)); _ptcolors.push_back(GsColor::green);

		_ptcoords.push_back(GsVec(0.125, 0.1546, 0.0)); _ptcolors.push_back(GsColor::blue);
		_ptcoords.push_back(GsVec(-0.432, -0.7623, 0.0)); _ptcolors.push_back(GsColor::blue);
		_ptcoords.push_back(GsVec(0.8432, -0.2146, 0.0)); _ptcolors.push_back(GsColor::blue);
		_ptcoords.push_back(GsVec(-0.3256, 0.7423, 0.0)); _ptcolors.push_back(GsColor::blue);
		
		_ptcoords.push_back(GsVec(0.742, 0.215, 0.0)); _ptcolors.push_back(GsColor::orange);
		_ptcoords.push_back(GsVec(-0.325, -0.356, 0.0)); _ptcolors.push_back(GsColor::orange);
		_ptcoords.push_back(GsVec(0.1245, -0.853, 0.0)); _ptcolors.push_back(GsColor::orange);
		_ptcoords.push_back(GsVec(-0.4631, 0.12445, 0.0)); _ptcolors.push_back(GsColor::orange);
		
		_ptcoords.push_back(GsVec(0.8259, 0.3529, 0.0)); _ptcolors.push_back(GsColor::yellow);
		_ptcoords.push_back(GsVec(-0.4593, -0.5230, 0.0)); _ptcolors.push_back(GsColor::yellow);
		_ptcoords.push_back(GsVec(0.42150, -0.3560, 0.0)); _ptcolors.push_back(GsColor::yellow);
		_ptcoords.push_back(GsVec(-0.2560, 0.3620, 0.0)); _ptcolors.push_back(GsColor::yellow);

		_ptcoords.push_back(GsVec(0.2560, 0.90327, 0.0)); _ptcolors.push_back(GsColor::lightblue);
		_ptcoords.push_back(GsVec(-0.74, -0.9602, 0.0)); _ptcolors.push_back(GsColor::lightblue);
		_ptcoords.push_back(GsVec(0.3607, -0.3275, 0.0)); _ptcolors.push_back(GsColor::lightblue);
		_ptcoords.push_back(GsVec(-0.9258, 0.92356, 0.0)); _ptcolors.push_back(GsColor::lightblue);
		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _pts.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _ptcoords.size() * 3 * sizeof(float), &_ptcoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _pts.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _ptcolors.size() * 4 * sizeof(gsbyte), &_ptcolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_pts.changed = false;
	}

	// Define some triangles:
	if (_tris.changed) // needs update
	{
		double timestart = GetCurrentTime();
		double g = cos(timestart/500);
		_tricoords.clear(); _tricolors.clear();
		// Encode some overlapping triangles, red:
		_tricoords.push_back(GsVec(-0.7+g/10, -0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.6+g/10, -0.5, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-.7+g/10, 0.6, 0.0));  _tricolors.push_back(GsColor::red);

		_tricoords.push_back(GsVec(-.7 + g / 10, 0.5, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.5 + g / 10, -0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.7 + g / 10, 0.6, 0.0));  _tricolors.push_back(GsColor::red);

		_tricoords.push_back(GsVec(-0.5 + g / 10, 0.6, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.5 + g / 10, -0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.48 + g / 10, -0.48, 0.0));  _tricolors.push_back(GsColor::red);

		// green:
		_tricoords.push_back(GsVec(-0.4 + g / 10, -0.5 + g / 10, 0.1)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.3 + g / 10, -0.5 + g / 10, -0.1)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.35 + g / 10, 0.3 + g / 10, -0.1)); _tricolors.push_back(GsColor::green);
		
		_tricoords.push_back(GsVec(-0.4 + g / 10, 0.35 + g / 10, 0.1)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.3 + g / 10, 0.35 + g / 10, -0.1)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.35 + g / 10, 0.45 + g / 10, -0.1)); _tricolors.push_back(GsColor::green);
		
		// blue:
		_tricoords.push_back(GsVec(0.0 - g / 10, 0.45 - g / 10, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.0 - g / 10, 0.3 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(-0.2 - g / 10, 0.0 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.0 - g / 10, -0.5 - g / 10, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.0 - g / 10, -0.4 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(-0.2 - g / 10, 0.0 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.0 - g / 10, 0.45 - g / 10, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.0 - g / 10, 0.3 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.2 - g / 10, .37 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.0 - g / 10, -0.5 - g / 10, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.0 - g / 10, -0.4 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.2 - g / 10, -.45 - g / 10, -0.1)); _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.3, -0.5 + g / 10, 0.0)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.4, -0.5 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.35, 0.5 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);

		_tricoords.push_back(GsVec(0.35, -0.1 + g / 10, 0.0)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.35, -0.0 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.675, -0.05 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);

		_tricoords.push_back(GsVec(0.7, -0.5 + g / 10, 0.0)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.6, -0.5 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);
		_tricoords.push_back(GsVec(0.65, 0.5 + g / 10, -0.1)); _tricolors.push_back(GsColor::magenta);

		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _tris.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _tricoords.size() * 3 * sizeof(float), &_tricoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _tris.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _tricolors.size() * 4 * sizeof(gsbyte), &_tricolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_tris.changed = false;
	}
 }

void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Update objects if needed:
   buildObjects(.05);

   // Define some identity transformations; our shaders require them but in this
   // example support code we do not need to use them, so just let them be GsMat::id:
   GsMat vtransf(GsMat::id), vproj(GsMat::id); // transformations

   // Draw Lines:
   glUseProgram ( _lines.prog );
   glBindVertexArray ( _lines.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _lines.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _lines.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _lines.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _lines.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_LINES, 0, _linecoords.size() );
   // (see documentation at: https://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml)

   // Draw Points:
   glUseProgram ( _pts.prog );
   glBindVertexArray ( _pts.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _pts.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _pts.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _pts.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _pts.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_POINTS, 0, _ptcoords.size() );

   // Draw Triangles:
   glUseProgram ( _tris.prog );
   glBindVertexArray ( _tris.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _tris.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _tris.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _tris.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _tris.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_TRIANGLES, 0, _tricoords.size() );

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

