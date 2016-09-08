
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_capsule.h"
# include "light.h"
#include "normals.h"
#include "material.h"



// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { 


 private :
    // OpenGL shaders and programs:
 GlShader _vertexsh, _fragsh, _gouradshader,_fragsh2;
    GlProgram _prog, _prog2;


    // My scene objects:
    SoAxis _axis;

	
	
	//capsule added as _cap
	SoCapsule _cap;
	
	//add normals
	normals no;
	
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    bool  _viewaxis;
    int _w, _h;
	
	
   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    GsVec2 windowToScene ( const GsVec2& v );
	double nfaces = 5.0;
	double len = 1.0;
	int normtype;
   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y);
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	light l = light(GsVec(1, 0, 1), GsColor::white, GsColor::white, GsColor::white);
	mat m = mat(GsVec(1, 1, 1), GsColor::black, GsColor::blue, GsColor::blue, float(3.182));
	int normson = 1;
};

#endif // APP_WINDOW_H
