
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
	rhand.load("rhand.m");
	rlowerarm.load("rlowerarm.m");
	rupperarm.load("rupperarm.m");
}

void normals::buildupperarm(double nfaces, int normtype)
{

	double scale = 2.0;
	if (normtype == 0) {
		for (int i = 0; i < rupperarm.vsize; i++)

		{
			N.push_back(rupperarm.V[i] / scale);
			N.push_back((rupperarm.V[i] / scale + (rupperarm.N[i] / 10 / scale)));
			C.push_back(GsColor::red);
			C.push_back(GsColor::red);
		}
	}

	else
	{
		for (int i = 0; i < rupperarm.fsize; i++) {
			GsVec a = (rupperarm.V[rupperarm.F[i].va] + rupperarm.V[rupperarm.F[i].vb] + rupperarm.V[rupperarm.F[i].vc]) / 3;
			N.push_back(a / scale);
			N.push_back(a / scale + (rupperarm.N[rupperarm.F[i].na]) / (10 * scale));
			C.push_back(GsColor::red);
			C.push_back(GsColor::red);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);

	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = N.size();

	// free non-needed memory:
	N.resize(0); C.resize(0);
}


void normals::buildlowerarm(double nfaces, int normtype)
{

	double scale = 2.0;
	if (normtype == 0) {
		for (int i = 0; i < rlowerarm.vsize; i++)

		{
			N.push_back(rlowerarm.V[i] / scale);
			N.push_back((rlowerarm.V[i] / scale + (rlowerarm.N[i] / 10 / scale)));
			C.push_back(GsColor::red);
			C.push_back(GsColor::red);
		}
	}

	else
	{
		for (int i = 0; i < rlowerarm.fsize; i++) {
			GsVec a = (rlowerarm.V[rlowerarm.F[i].va] + rlowerarm.V[rlowerarm.F[i].vb] + rlowerarm.V[rlowerarm.F[i].vc]) / 3;
			N.push_back(a / scale);
			N.push_back(a / scale + (rlowerarm.N[rlowerarm.F[i].na]) / (10 * scale));
			C.push_back(GsColor::red);
			C.push_back(GsColor::red);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);

	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = N.size();

	// free non-needed memory:
	N.resize(0); C.resize(0);
}

void normals::buildhand(double nfaces, int normtype)
{
	
	double scale = 2.0;
	if (normtype == 0) {
		for (int i = 0; i < rhand.vsize; i++)

		{
			N.push_back(rhand.V[i] / scale);
			N.push_back((rhand.V[i] / scale + (rhand.N[i] / 10 / scale)));
			C.push_back(GsColor::red);
			C.push_back(GsColor::red);
		}
	}

		else
		{
			for (int i = 0; i < rhand.fsize; i++) {
				GsVec a = (rhand.V[rhand.F[i].va] + rhand.V[rhand.F[i].vb] + rhand.V[rhand.F[i].vc]) / 3;
				N.push_back(a / scale);
				N.push_back(a/scale + (rhand.N[rhand.F[i].na]) / (10 *scale));
				C.push_back(GsColor::red);
				C.push_back(GsColor::red);
			}
		}
	

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);

	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = N.size();

	// free non-needed memory:
	N.resize(0); C.resize(0);
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
