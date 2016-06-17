
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "light.h"
#include "normals.h"
#include "material.h"
#include "model.h"
#include "so_model.h"
#include "so_capsule.h"
#include "math.h"
#include "floor.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { 


 private :
    // OpenGL shaders and programs:
 GlShader _vertexsh, _fragsh, _gouradshader,_fragsh2,_gouradshader_tex, _fragsh2_tex;
    GlProgram _prog, _prog2,_prog3;


    // My scene objects:
    SoAxis _axis;
	SoModel _wheel1;
	SoModel _body;
	SoModel _wheel2;
	SoModel _tail;
	SoModel _head;
	SoFloor _floor;
	SoCapsule _capsule;
	//add normals
	normals nohand, noupper, nolower;
	
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    bool  _viewaxis;
	bool _viewmodel1, _viewmodel2, _viewmodel3;
    int _w, _h;
	
	
   public :

	   GsMat camview, persp, sproj,fakecam;
	   GsVec eye, center, up;
	   bool space = TRUE, cam = TRUE;
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
	GsMat shadowMatrix();
	float xShadow=1, yShadow=1, zShadow=1;
    GsVec2 windowToScene ( const GsVec2& v );
	double nfaces = 5.0;
	double len = 1.0, shoulder=0, elbow=0,hand=0,xmove = 0, ymove=0,zmove=0,imroty = 0,armrotx = 0,scalex = 1,scaley=1,scalez=1,leg=0,foot=0,footy=0;
	int normtype = 0;
   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y);
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	light l = light(GsVec(1, 0, 1), GsColor::white, GsColor::white, GsColor::white);
	mat m = mat(GsVec(20, 0, 0), GsColor::magenta, GsColor::magenta, GsColor::magenta, float(.5));
	mat m2 = mat(GsVec(20, 0, 0), GsColor::blue, GsColor::blue, GsColor::blue, float(.5));
	mat m3 = mat(GsVec(20, 0, 0), GsColor::lightblue, GsColor::lightblue, GsColor::lightblue, float(.5));
	mat m4 = mat(GsVec(20, 0, 0), GsColor::darkgray, GsColor::darkgray, GsColor::darkgray, float(.5));
	mat m5 = mat(GsVec(20, 0, 0), GsColor::black, GsColor::black, GsColor::black, float(.5));
	bool normson = 1;
};

#endif // APP_WINDOW_H
