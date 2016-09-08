

// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCapsule : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> C; // normals
	int _numpoints;         // saves the number of points

public:
	SoCapsule();
	void init(const GlProgram& prog);
	void build(double nfaces, int normtype);
	void draw(const GsMat& tr, const GsMat& pr, const light& l, const mat & m);
};



#endif // SO_AXIS_H