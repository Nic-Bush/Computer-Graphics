

// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODEL_H
#define SO_MODEL_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "math.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoModel : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> N; // normals
	int _numpoints;         // saves the number of points
	Model rhand;
	Model rlowerarm;
	Model rupperarm;


public:
	SoModel();
	void init(const GlProgram& prog);
	void buildwheel(double nfaces, int normtype);
	void buildBody(double nfaces, int normtype);
	void buildfloor(double nfaces, int normtype);
	void draw(const GsMat& tr, const GsMat& pr, const light& l, const mat & m);
};



#endif // SO_AXIS_H