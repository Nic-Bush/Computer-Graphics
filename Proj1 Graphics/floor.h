#pragma once
#ifndef SO_Floor
#define SO_Floor

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "math.h"
#include <gsim/gs_image.h>

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoFloor : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> N; // normals
	std::vector<GsVec2> T;
	int _numpoints;         // saves the number of point


public:
	GsImage I;
	gsuint id;
	SoFloor();
	void init(const GlProgram& prog);
	void buildfloor(double nfaces, int normtype);
	void draw(const GsMat& tr, const GsMat& pr, const light& l, const mat & m);
};



#endif // SO_AXIS_H