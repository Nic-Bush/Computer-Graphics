
# include "so_capsule.h"

SoCapsule::SoCapsule() {
	_numpoints = 0;
}

void SoCapsule::init(const GlProgram& prog) {
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}

void SoCapsule::build(float len, float rt, float rb, int nfaces) {
	P.clear(); C.clear();
    P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below
	// circle radius rt, vetex at cos, sin, -len/2
		
	for (int p = 0; p < 360; p += 360 / nfaces) {
	}
		for (int i = 0; i <= nfaces; i++) {//for all faces
			float phi = 2.0 * M_PI * i / nfaces;//find the x angle

			P.push_back(GsVec(rb*cos(phi), -len / 2, rb*sin(phi)));//create bottom set of the central triangles
			P.push_back(GsVec(rt*cos(phi), len / 2, rt*sin(phi)));
			P.push_back(GsVec(rb*cos(phi + 2.0 * PI / nfaces), -len / 2.0, rb*sin(phi + 2.0 * PI / nfaces)));
			for (int m = 0; m < 3; m++) C.push_back(GsColor::magenta);

			P.push_back(GsVec(rb*cos(phi + 2.0 * PI / nfaces), -len / 2.0, rb*sin(phi + 2.0 * PI / nfaces)));//create the top set of central triangles 
			P.push_back(GsVec(rt*cos(phi + 2.0 * PI / nfaces), len / 2.0, rt*sin(phi + 2.0 * PI / nfaces)));
			P.push_back(GsVec(rt*cos(phi), len / 2, rt*sin(phi)));
			for (int m = 0; m < 3; m++) C.push_back(GsColor::darkred);
			for (int j = 0; j <= nfaces / 2; j++) {
				float theta = PI * (double)j / (double)nfaces;
				if (j == (int)(nfaces / 2)) {//if final itter
					P.push_back(GsVec(rb*sin(theta)*cos(phi), -rb*cos(theta) - len / 2, rb*sin(theta)*sin(phi)));
					P.push_back(GsVec(rb*cos(phi), -len / 2, rb*sin(phi)));
					P.push_back(GsVec(rb*sin(theta)*cos(phi + 2.0 * PI / nfaces), -rb*cos(theta) - len / 2.0, rb*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
					for (int m = 0; m < 3; m++) C.push_back(GsColor::blue);//lower sphere of circle
					P.push_back(GsVec(rb*cos(phi), -len / 2, rb*sin(phi)));
					P.push_back(GsVec(rb*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), -rb*cos(theta) - len / 2.0, rb*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
					P.push_back(GsVec(rb*cos(phi + 2.0 * M_PI / nfaces), -len / 2.0, (double)rb*sin(phi + 2.0 * M_PI / nfaces)));
					for (int m = 0; m < 3; m++) C.push_back(GsColor::cyan);//lower part of circle
				}
				else {
					P.push_back(GsVec(rb*sin(theta)*cos(phi), -rb*cos(theta) - len / 2, rb*sin(theta)*sin(phi)));
					P.push_back(GsVec(rb*sin(theta + M_PI / nfaces)*cos(phi), -rb*cos(theta + M_PI / nfaces) - len / 2, rb*sin(theta + M_PI / nfaces)*sin(phi)));
					P.push_back(GsVec(rb*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), (double)-rb*cos(theta) - len / 2, rb*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
					for (int m = 0; m < 3; m++) C.push_back(GsColor::blue);//make more triangles
					P.push_back(GsVec(rb*sin(theta + M_PI / nfaces)*cos(phi), -rb*cos(theta + M_PI / nfaces) - len / 2, rb*sin(theta + M_PI / nfaces)*sin(phi)));
					P.push_back(GsVec(rb*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), (double)-rb*cos(theta) - len / 2, rb*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
					P.push_back(GsVec(rb*sin(theta + M_PI / nfaces)*cos(phi + 2.0 * M_PI / nfaces), -rb*cos(theta + M_PI / nfaces) - len / 2, (double)rb*sin(theta + M_PI / nfaces)*sin(phi + 2.0 * M_PI / nfaces)));
					for (int m = 0; m < 3; m++) C.push_back(GsColor::cyan);//triangles
				}
				for (int j = 0; j <= nfaces / 2; j++) {//for top part same comments as above just with top part in it.... i guess
					float theta = M_PI * (double)j / (double)nfaces;
					if (j == (int)(nfaces / 2)) {
						P.push_back(GsVec(rt*sin(theta)*cos(phi), rt*cos(theta) + len / 2, rt*sin(theta)*sin(phi)));
						P.push_back(GsVec(rt*cos(phi), len / 2, rt*sin(phi)));
						P.push_back(GsVec(rt*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), rt*cos(theta) + len / 2.0, rt*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
						for (int m = 0; m < 3; m++) C.push_back(GsColor::blue);
						P.push_back(GsVec(rt*cos(phi), len / 2, rt*sin(phi)));
						P.push_back(GsVec(rt*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), rt*cos(theta) + len / 2.0, rt*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
						P.push_back(GsVec(rt*cos(phi + 2.0 * M_PI / nfaces), len / 2.0, (double)rt*sin(phi + 2.0 * M_PI / nfaces)));
						for (int m = 0; m < 3; m++) C.push_back(GsColor::cyan);
					}
					else {
						P.push_back(GsVec(rt*sin(theta)*cos(phi), rt*cos(theta) + len / 2, rt*sin(theta)*sin(phi)));
						P.push_back(GsVec(rt*sin(theta + M_PI / nfaces)*cos(phi), rt*cos(theta + M_PI / nfaces) + len / 2, rt*sin(theta + M_PI / nfaces)*sin(phi)));
						P.push_back(GsVec(rt*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), (double)rt*cos(theta) + len / 2, rt*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
						for (int m = 0; m < 3; m++) C.push_back(GsColor::blue);
						P.push_back(GsVec(rt*sin(theta + M_PI / nfaces)*cos(phi), rt*cos(theta + M_PI / nfaces) + len / 2, rt*sin(theta + M_PI / nfaces)*sin(phi)));
						P.push_back(GsVec(rt*sin(theta)*cos(phi + 2.0 * M_PI / nfaces), (double)rt*cos(theta) + len / 2, rt*sin(theta)*sin(phi + 2.0 * M_PI / nfaces)));
						P.push_back(GsVec(rt*sin(theta + M_PI / nfaces)*cos(phi + 2.0 * M_PI / nfaces), rt*cos(theta + M_PI / nfaces) + len / 2, (double)rt*sin(theta + M_PI / nfaces)*sin(phi + 2.0 * M_PI / nfaces)));
						for (int m = 0; m < 3; m++) C.push_back(GsColor::cyan);
					}
				}
			}
		
			
		int n = 0;
		
	}
	// circle radius rb, vertex at cos, sin, len/2

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	_numpoints = P.size();
	P.resize(0); C.resize(0);
}

void SoCapsule::draw(GsMat& tr, GsMat& pr) {
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

