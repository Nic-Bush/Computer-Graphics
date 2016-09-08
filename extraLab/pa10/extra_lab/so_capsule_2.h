#define _USE_MATH_DEFINES
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_2H
#define SO_CAPSULE_2H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include <gsim/gs.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"
# include "math.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCapsule_2 : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> C; // colors
	int _numpoints;         // saves the number of points
	double PI = M_PI;
public:
	SoCapsule_2();
	void init(const GlProgram& prog);
	void build(float len, float rt, float rb, int nfaces);
	void draw(GsMat& tr, GsMat& pr,const light&l, const mat& m);
	void subdiv(std::vector<GsVec>& Q, int degree);
};

#endif // SO_AXIS_H