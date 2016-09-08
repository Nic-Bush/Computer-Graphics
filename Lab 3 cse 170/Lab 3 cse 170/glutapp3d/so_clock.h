#pragma once
#define _USE_MATH_DEFINES
// Ensure the header file is included only once in multi-file projects
#ifndef so_clock_H
#define so_clock_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include <gsim/gs.h>
# include "ogl_tools.h"
# include "math.h"
# include "so_capsule.h"
# include "so_cylinder.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Soclock : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	SoCapsule longHand, shortHand;
	SoCylinder watchrim;
	int _numpoints;         // saves the number of points
	float longhandLen = 0.3, shorthandLen = 0.2, extravar1 = 0.04, extravar2 = 0.07, numSides = 10;

public:
	Soclock();
	void init(const GlProgram& prog);
	void build(float r);
	void draw(GsMat& tr, GsMat& pr,double minutes, double seconds);
};


#endif // SO_STOPWATCH_H