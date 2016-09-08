
# include "normals.h"

normals::normals()
{
	_numpoints = 0;
}

void normals::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}

void normals::build(double nfaces, int normtype)
{
	int i;
	double rad = .5;
	double angularLength = (2.0 * 3.14159) / (double)nfaces;
	if (normtype == 1) {
		for (int k = 0; k < nfaces; k++) {
			double theta = angularLength * (double)k;

			/*

					P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), .7, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);
					P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), -.3, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);
				//	P.push_back(GsVec((rad)*cos(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180)), .5, (rad)*cos(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180)))); C.push_back(GsColor::blue);
				*/
				/*//1st triangle
				GsVec t((rad)*cos(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180)), .5, (rad)*sin(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180))); C.push_back(GsColor::white);
				GsVec s((rad)*cos(k*(pi / 180.0)), -.5, (rad)*sin(k*(pi / 180.0))); C.push_back(GsColor::white);
				GsVec v((rad)*cos(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180)), -.5, (rad)*sin(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180))); C.push_back(GsColor::white);

				//second triangle
				GsVec a((rad)*cos(k*(pi / 180.0)), .5, (rad)*sin(k*(pi / 180.0))); C.push_back(GsColor::white);
				GsVec b((rad)*cos(k*(pi / 180.0)), -.5, (rad)*sin(k*(pi / 180.0))); C.push_back(GsColor::white);
				GsVec c((rad)*cos(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180)), .5, (rad)*sin(k*(pi / 180.0) + (360.0 / nfaces)*(pi / 180))); C.push_back(GsColor::white);
				*/

				/*push_back(GsVec(2*(rad)*cos(k*(pi / 180.0)+pi/nfaces) , 0.0, 2*(rad)*sin(k*(pi / 180.0)+pi/nfaces))); C.push_back(GsColor::white);
				P.push_back(GsVec(0.0 , 0.0, 0.0)); C.push_back(GsColor::white);

				P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), 0.5, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);
				P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), 1.0, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);
				P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), -0.5, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);
				P.push_back(GsVec((rad)*cos(k*(pi / 180.0)), -1.0, (rad)*sin(k*(pi / 180.0)))); C.push_back(GsColor::white);*/

			GsVec a(rad * cos(theta), 0.5, rad * sin(theta));
			GsVec b(rad * cos(theta), -0.5, rad * sin(theta));
			GsVec n(cos(theta), 0.0, sin(theta));

			P.push_back(a); C.push_back(GsColor::white);
			P.push_back(a + (0.05 * n)); C.push_back(GsColor::white);
			P.push_back(b); C.push_back(GsColor::white);
			P.push_back(b + (0.05 * n)); C.push_back(GsColor::white);

			P.push_back(a); C.push_back(GsColor::white);
			P.push_back(a + (0.05 * GsVec(0.0, 1.0, 0.0))); C.push_back(GsColor::white);
			P.push_back(b); C.push_back(GsColor::white);
			P.push_back(b + (0.05 * GsVec(0.0, -1.0, 0.0))); C.push_back(GsColor::white);


			/*

			GsVec n = s - t;

			GsVec m = v - t;

			normal(n, m, c); C.push_back(GsColor::white);

			//normal(a, b, c); C.push_back(GsColor::white);
		*/
		//N.push_back((0.2*normal(normal(t, s, v).i, normal(t, s, v).j, normal(t, s, v).k))); C.push_back(GsColor::white);
	//	N.push_back((normal(normal(a, b, c).i, normal(a, b, c).j, normal(a, b, c).k))); C.push_back(GsColor::white);


		}
	}
	else {
		for (int k = 0; k < nfaces; k++) {
			double theta = angularLength * (double)k;

			GsVec a(rad * cos(theta), 0.5, rad * sin(theta));
			GsVec b(rad * cos(theta), -0.5, rad * sin(theta));
			GsVec c(rad * cos(theta + angularLength), .5 , rad * sin(theta + angularLength));
			GsVec d(rad * cos(theta + angularLength), -.5, rad * sin(theta + angularLength));
			
			GsVec e = (a + b + c) / 3;
			GsVec f = (b + c + d) / 3;

			P.push_back(a); C.push_back(GsColor::white);
			P.push_back(a + (0.05 * GsVec(0.0, 1.0, 0.0))); C.push_back(GsColor::white);
			P.push_back(b); C.push_back(GsColor::white);
			P.push_back(b + (0.05 * GsVec(0.0, -1.0, 0.0))); C.push_back(GsColor::white);

			P.push_back(e + GsVec(.5*(rad)*cos(theta + angularLength / 2 ), 0.0, .5*(rad)*sin(theta + angularLength / 2))); C.push_back(GsColor::white);
			P.push_back(e); C.push_back(GsColor::white);
			P.push_back(f + GsVec(.5*(rad)*cos(theta + angularLength / 2), 0.0, .5* (rad)*sin(theta + angularLength / 2))); C.push_back(GsColor::white);
			P.push_back(f); C.push_back(GsColor::white);

		}
	}




	GsVec a(0.0, 0.5, 0.0);
	GsVec b(0.0, -0.5, 0.0);

	P.push_back(a); C.push_back(GsColor::white);
	P.push_back(a + (0.05 * GsVec(0.0, 1.0, 0.0))); C.push_back(GsColor::white);
	P.push_back(b); C.push_back(GsColor::white);
	P.push_back(b + (0.05 * GsVec(0.0, -1.0, 0.0))); C.push_back(GsColor::white);

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0);
}

void normals::draw(GsMat& tr, GsMat& pr)
{
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

	glDrawArrays(GL_LINES, 0, _numpoints);

	//draw the things
}
