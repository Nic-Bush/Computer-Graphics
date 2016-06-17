#include "floor.h"

SoFloor::SoFloor()
{
	_numpoints = 0;
}

void SoFloor::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(3);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(11); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(10, "Tex1");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ka");
	uniform_location(7, "kd");
	uniform_location(8, "ks");
	uniform_location(9, "sh");
	GsImage I;
	if (!I.load("../fuka.bmp"))std::cout << "could not load image\n";
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindVertexArray(0);
	I.init(0,0);// free image from CPU

}


void SoFloor::buildfloor(double nfaces, int normtype)
{
	P.push_back(GsVec(25, 0, 25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1,1));
	P.push_back(GsVec(-25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0, 0));
	P.push_back(GsVec(-25, 0, 25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0, 1));

	P.push_back(GsVec(-25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(0, 0));
	P.push_back(GsVec(25, 0, -25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1, 0));
	P.push_back(GsVec(25, 0, 25));
	N.push_back(GsVec(0, 1, 0));
	T.push_back(GsVec2(1, 1));

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);// false means no normalization 

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); N.resize(0);
}
void SoFloor::draw(const GsMat& tr, const GsMat& pr, const light&l, const mat& m)
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

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // colors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

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
	glUniform1uiv(uniloc[10], 1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	//draw the things
}
