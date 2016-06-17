
# include "so_model.h"

SoModel::SoModel()
{
	_numpoints = 0;
}

void SoModel::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(10); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ka");
	uniform_location(7, "kd");
	uniform_location(8, "ks");
	uniform_location(9, "sh");	
	
	rhand.load("rhand.m");
	rlowerarm.load("rlowerarm.m");
	rupperarm.load("rupperarm.m");

}

void SoModel::buildwheel(double nfaces, int normtype)
{
	double step = 2 * 3.141592/3;//full circle of 2 pi over number of faces
	for (int i = 0; i < 3; i++)
	{
		P.push_back(GsVec(.5,sin(i*step),cos(i * step)));
		N.push_back(GsVec(0.0,sin((i + 1/2) *step),cos((i + 1/2) * step)));
		P.push_back(GsVec(.5, sin((i+1)*step), cos((i+1 )* step)));
		N.push_back(GsVec(0.0, sin((i + 1 / 2) *step), cos((i + 1 / 2) * step)));
		P.push_back(GsVec(-.5, sin(i*step), cos(i * step)));
		N.push_back(GsVec(0.0, sin((i + 1 / 2) *step), cos((i + 1 / 2) * step)));

		P.push_back(GsVec(-.5, sin(i*step), cos(i * step)));
		N.push_back(GsVec(0.0, sin((i + 1 / 2) *step), cos((i + 1 / 2) * step)));
		P.push_back(GsVec(-.5, sin((i+1)*step), cos((i+1) * step)));
		N.push_back(GsVec(0.0, sin((i + 1 / 2) *step), cos((i + 1 / 2) * step)));
		P.push_back(GsVec(.5, sin((i+1)*step), cos((i+1) * step)));
		N.push_back(GsVec(0.0, sin((i + 1 / 2) *step), cos((i + 1 / 2) * step)));
	}
	P.push_back(GsVec(-.5, sin(0), cos(0)));
	N.push_back(GsVec(-1,0,0));
	P.push_back(GsVec(-.5, sin(step), cos(step)));
	N.push_back(GsVec(-1, 0, 0));
	P.push_back(GsVec(-.5, sin(2*step), cos(2*step)));
	N.push_back(GsVec(-1, 0, 0));

	P.push_back(GsVec(.5, sin(0), cos(0)));
	N.push_back(GsVec(1, 0, 0));
	P.push_back(GsVec(.5, sin(step), cos(step)));
	N.push_back(GsVec(1, 0, 0));
	P.push_back(GsVec(.5, sin(2 * step), cos(2 * step)));
	N.push_back(GsVec(1, 0, 0));
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); N.resize(0);
}

void SoModel::buildBody(double nfaces, int normtype)
{
	double step = 2 * 3.141592 / 15;//full circle of 2 pi over number of faces
	for (int i = 0; i < 15; i++)
	{
		P.push_back(GsVec( sin(i*step), cos(i * step),.5));
		N.push_back(GsVec( sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));
		P.push_back(GsVec( sin((i + 1)*step), cos((i + 1)* step),.5));
		N.push_back(GsVec(sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));
		P.push_back(GsVec(sin(i*step), cos(i * step),-.5));
		N.push_back(GsVec(sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));

		P.push_back(GsVec( sin(i*step), cos(i * step),-.5));
		N.push_back(GsVec( sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));
		P.push_back(GsVec( sin((i + 1)*step), cos((i + 1) * step),-.5));
		N.push_back(GsVec(sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));
		P.push_back(GsVec( sin((i + 1)*step), cos((i + 1) * step),.5));
		N.push_back(GsVec( sin((i + 1 / 2) *step), cos((i + 1 / 2) * step),0.0));
	
		P.push_back(GsVec(0.0,0.0,-.5));
		N.push_back(GsVec(0, 0, -1));
		P.push_back(GsVec(sin(i * step),cos(i * step),-.5));
		N.push_back(GsVec(0, 0, -1));
		P.push_back(GsVec(sin((i+1) * step), cos((i+1) * step), -.5));
		N.push_back(GsVec(0, 0, -1));

		P.push_back(GsVec(0.0, 0.0, .5));
		N.push_back(GsVec(0, 0, 1));
		P.push_back(GsVec(sin(i * step), cos(i * step), .5));
		N.push_back(GsVec(0, 0, 1));
		P.push_back(GsVec(sin((i + 1) * step), cos((i + 1) * step), .5));
		N.push_back(GsVec(0, 0, 1));

	}

	// send data to OpenGL buffers:
	



	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); N.resize(0);
}

void SoModel::buildfloor(double nfaces, int normtype)
{
	P.push_back(GsVec(25,0,25));
	N.push_back(GsVec(0,1,0));
	P.push_back(GsVec(-25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	P.push_back(GsVec(-25, 0, 25));
	N.push_back(GsVec(0, 1, 0));

	P.push_back(GsVec(-25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	P.push_back(GsVec(25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	P.push_back(GsVec(25, 0, 25));
	N.push_back(GsVec(0, 1, 0));

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); N.resize(0);
}



void SoModel::draw(const GsMat& tr, const GsMat& pr, const light&l, const mat& m)
{
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; // we convert below our color values to be in [0,1]   
	glUniform3fv(uniloc[2], 1, l.pos.e);
	glUniform4fv(uniloc[3], 1, l.amb.get(f));
	glUniform4fv(uniloc[4], 1, l.dif.get(f));
	glUniform4fv(uniloc[5], 1, l.spe.get(f));
	glUniform4fv(uniloc[6], 1, m.amb.get(f));
	glUniform4fv(uniloc[7], 1, m.dif.get(f));
	glUniform4fv(uniloc[8], 1, m.spe.get(f));
	glUniform1fv(uniloc[9], 1, &m.shine);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	//draw the things
}
