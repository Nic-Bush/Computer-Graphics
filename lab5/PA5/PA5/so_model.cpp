
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

void SoModel::buildhand(double nfaces, int normtype)
{

	double scale = 2.0;

	for (int i = 0; i < rhand.fsize; i++) {
		P.push_back((rhand.V[rhand.F[i].va]) / scale);
		P.push_back((rhand.V[rhand.F[i].vb]) / scale); 
		P.push_back((rhand.V[rhand.F[i].vc])/ scale);

		if (normtype == 0)
		{
			N.push_back((rhand.N[rhand.F[i].na]) / scale);
			N.push_back((rhand.N[rhand.F[i].nb]) / scale);
			N.push_back((rhand.N[rhand.F[i].nc]) / scale);
		}
		else
		{
			N.push_back((rhand.N[rhand.F[i].na]) / scale);
			N.push_back((rhand.N[rhand.F[i].na]) / scale);
			N.push_back((rhand.N[rhand.F[i].na]) / scale);
		}
	}



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

void SoModel::buildlowerarm(double nfaces, int normtype)
{

	double scale = 2.0;

	for (int i = 0; i < rlowerarm.fsize; i++) {
		P.push_back(rlowerarm.V[rlowerarm.F[i].va] / scale);
		P.push_back(rlowerarm.V[rlowerarm.F[i].vb] / scale);
		P.push_back(rlowerarm.V[rlowerarm.F[i].vc] /scale);

		if (normtype == 0) {
			N.push_back(rlowerarm.N[rlowerarm.F[i].na] / scale);
			N.push_back(rlowerarm.N[rlowerarm.F[i].nb] / scale);
			N.push_back(rlowerarm.N[rlowerarm.F[i].nc] / scale);
		}
		else {
			N.push_back(rlowerarm.N[rlowerarm.F[i].na] / scale);
			N.push_back(rlowerarm.N[rlowerarm.F[i].na] / scale);
			N.push_back(rlowerarm.N[rlowerarm.F[i].na] / scale);
		}
	}



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

void SoModel::buildupperarm(double nfaces, int normtype)
{

	double scale = 2.0;

	for (int i = 0; i < rupperarm.fsize; i++) {
		P.push_back(rupperarm.V[rupperarm.F[i].va] / scale);
		P.push_back(rupperarm.V[rupperarm.F[i].vb] / scale);
		P.push_back(rupperarm.V[rupperarm.F[i].vc] / scale);
		
		if (normtype == 0) {
			N.push_back(rupperarm.N[rupperarm.F[i].na] / scale);
			N.push_back(rupperarm.N[rupperarm.F[i].nb] / scale);
			N.push_back(rupperarm.N[rupperarm.F[i].nc] / scale);
		}
		else
		{
			N.push_back(rupperarm.N[rupperarm.F[i].na] / scale);
			N.push_back(rupperarm.N[rupperarm.F[i].na] / scale);
			N.push_back(rupperarm.N[rupperarm.F[i].na] / scale);
		}
	}



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
