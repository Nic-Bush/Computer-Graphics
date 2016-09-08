
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_capsule.h"
# include "so_capsule_2.h"
# include "light.h"
#include "material.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    GlShader _vertexsh, _fragsh, _gouradshader, _fragsh2;
    GlProgram _prog, _prog2;
    // My scene objects:
    SoAxis _axis;
	SoCapsule _capsule;
	SoCapsule_2 _capsule_2;
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    bool  _viewaxis;
    int _w, _h;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	light l = light(GsVec(1, 0, 1), GsColor::white, GsColor::white, GsColor::white);
	mat m = mat(GsVec(20, 0, 0), GsColor::magenta, GsColor::blue, GsColor::white, float(10));
	mat m2 = mat(GsVec(20, 0, 0), GsColor::black, GsColor::blue, GsColor::blue, float(.5));
	mat m3 = mat(GsVec(20, 0, 0), GsColor::black, GsColor::lightblue, GsColor::lightblue, float(.5));
};

#endif // APP_WINDOW_H
