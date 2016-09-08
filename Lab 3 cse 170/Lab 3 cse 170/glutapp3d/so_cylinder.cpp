
# include "so_cylinder.h"

SoCylinder::SoCylinder() {
	_numpoints = 0;
}

void SoCylinder::init(const GlProgram& prog) {
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}

void SoCylinder::build(float len, float rt, float rb, int nfaces) {
	P.clear(); C.clear();
	P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

	int numCapDivisions = int(nfaces / 3) + 1;
	for (int p = 0; p < 360; p += 360 / nfaces) {

		P.push_back(GsVec(rb*cos(p * (M_PI / 180.0)), rb*sin(p * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec(rt*cos(p * (M_PI / 180.0)), rt*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec(rb*cos((p + 360 / nfaces) * (M_PI / 180.0)), rb*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec(rb*cos((p + 360 / nfaces) * (M_PI / 180.0)), rb*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec(rt*cos((p + 360 / nfaces) * (M_PI / 180.0)), rt*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec(rt*cos(p * (M_PI / 180.0)), rt*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec((rb - 0.1)*cos(p * (M_PI / 180.0)), (rb - 0.1)*sin(p * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos(p * (M_PI / 180.0)), (rt - 0.1)*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec((rb - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rb - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec((rb - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rb - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rt - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos(p * (M_PI / 180.0)), (rt - 0.1)*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec(rb*cos(p * (M_PI / 180.0)), rb*sin(p * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec(rb*cos((p + 360 / nfaces) * (M_PI / 180.0)), rb*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec((rb - 0.1)*cos(p * (M_PI / 180.0)), (rb - 0.1)*sin(p * (M_PI / 180.0))+0.5, -len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);


		P.push_back(GsVec(rb*cos((p + 360 / nfaces) * (M_PI / 180.0)), rb*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec((rb - 0.1)*cos(p * (M_PI / 180.0)), (rb - 0.1)*sin(p * (M_PI / 180.0))+0.5, -len / 2.0));
		P.push_back(GsVec((rb - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rb - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, -len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec(rt*cos(p * (M_PI / 180.0)), rt*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rt - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos(p * (M_PI / 180.0)), (rt - 0.1)*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);

		P.push_back(GsVec(rt*cos((p + 360 / nfaces) * (M_PI / 180.0)), rt*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec(rt*cos(p * (M_PI / 180.0)), rt*sin(p * (M_PI / 180.0))+0.5, len / 2.0));
		P.push_back(GsVec((rt - 0.1)*cos((p + 360 / nfaces) * (M_PI / 180.0)), (rt - 0.1)*sin((p + 360 / nfaces) * (M_PI / 180.0))+0.5, len / 2.0));
		for (int m = 0; m < 3; m++) C.push_back(GsColor::green);
	}

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	_numpoints = P.size();
	P.resize(0); C.resize(0);
}

void SoCylinder::draw(GsMat& tr, GsMat& pr) {
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);


	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}

