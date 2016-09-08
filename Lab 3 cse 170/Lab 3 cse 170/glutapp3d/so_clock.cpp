
# include "so_clock.h"

Soclock ::Soclock() {
	_numpoints = 0;
}

void Soclock::init(const GlProgram& prog) {
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	watchrim.init(prog);
	longHand.init(prog);
	shortHand.init(prog);
}

void Soclock::build(float r) {
	P.clear(); C.clear();
	P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

	watchrim.build(0.05, 0.5, 0.5, 30);
	longHand.build(longhandLen, extravar1, extravar2, numSides);
	shortHand.build(shorthandLen, extravar1, extravar2, numSides);
	_numpoints = P.size();
	P.resize(0); C.resize(0);
}


void Soclock::draw(GsMat& tr, GsMat& pr,double minutes, double seconds) {
	watchrim.draw(tr, pr);
	
	GsMat oldTR = tr;
	
	GsMat transformmat = tr;
	transformmat.e11 = cos(seconds)/2;
	transformmat.e12 = -sin(seconds)/2;
	transformmat.e13 = 0;
	transformmat.e14 = 0;
	transformmat.e21 = sin(seconds)/2;
	transformmat.e22 = cos(seconds)/2;
	transformmat.e23 = 0;
	transformmat.e24 = .5;
	transformmat.e31 = 0;
	transformmat.e32 = 0;
	transformmat.e33 = 1;
	transformmat.e41 = 0;
	transformmat.e42 = 0;
	transformmat.e43 = 0;
	transformmat.e44 = 1;

	longHand.draw(tr*transformmat,pr);

	transformmat.e11 = cos(minutes)/2;
	transformmat.e12 = -sin(minutes)/2;
	transformmat.e21 = sin(minutes)/2;
	transformmat.e22 = cos(minutes)/2;
	shortHand.draw(tr*transformmat,pr);
}
