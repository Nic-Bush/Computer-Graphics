#pragma once


// Ensure the header file is included only once in multi-file projects
#ifndef NORMALS_H
#define NORMALS_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "math.h"
#include "model.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class normals: public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec>   N; // coordinates
	std::vector<GsColor> C; // colors
	int _numpoints;         // saves the number of points
	double pi = 3.14159265358979323846;
public:
	Model rhand, rlowerarm, rupperarm;
	normals();
	void init(const GlProgram& prog);
	void buildhand(double nfaces, int normtype);
	void buildlowerarm(double nfaces, int normtype);
	void buildupperarm(double nfaces, int normtype);
	void draw(GsMat& tr, GsMat& pr);
};



#endif 