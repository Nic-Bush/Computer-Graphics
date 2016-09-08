
// Ensure the header file is included only once in multi-file projects
#ifndef CURVE_EVAL_H
#define CURVE_EVAL_H

// Include needed header files
# include <vector>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class CurveEval : public GlObjects
{
private:
	GsArray<GsVec>   P; // coordinates
	GsArray<GsVec>	 Q;	// 3D curve
	GsArray<GsVec>	 S;	// 3D curve
	GsArray<GsVec>	 B;	// 3D curve
	GsArray<GsVec>	 CR;	// 3D curve
	GsArray<GsVec>	 BO;	// 3D curve
	GsArray<GsColor> C; // colors
	std::vector<GsVec> N; // normals
    int _numpoints;     // just saves the number of points

public:
	CurveEval();

	GsArray<GsVec> eval_bezier(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	GsArray<GsVec> eval_lagrange(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions);


	/*! B?Spline order k, n=pnts.size()?1;
	    For order k=3, (degree 2, quadratic case): t in [2,n+1]  */
	GsArray<GsVec> eval_bspline(float t, int k, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	/*! Evalutes a Catmull?Rom cubic spline, n=pnts.size()?1, t in [0,n?2] */
	GsArray<GsVec> eval_crspline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	/*! Evalutes a Bessel?Overhauser spline, n=pnts.size()?1, t in [0,n?2] */
	GsArray<GsVec> eval_bospline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions);

	GsArray<GsVec> setGuide(float t, const GsArray<GsVec> &ctrlpnts, bool smoothShading);

	void subdivBez(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	void subdivGrange(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	//void subdivBSpline(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	void subdivBSpline(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int degree, int numIt);
	float subdivBSpline(float location, int currentloc, int degree);
	//void subdivCRSpline(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	GsArray<GsVec> subdivCRSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	GsArray<GsVec> subdivBOSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions);
	void init(const GlProgram& prog);
	void build(float r);
	void draw(GsMat& tr, GsMat& pr, const light& l, const mat & m);
};

#endif // CURVE_EVAL_H
