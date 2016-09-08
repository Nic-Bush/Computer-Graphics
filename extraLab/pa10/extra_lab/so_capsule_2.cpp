
# include "so_capsule_2.h"

SoCapsule_2::SoCapsule_2() {
	_numpoints = 0;
}

void SoCapsule_2::init(const GlProgram& prog) {
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
}

void SoCapsule_2::build(float len, float rt, float rb, int degree) {
	P.clear(); C.clear();
    P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below
	// circle radius rt, vetex at cos, sin, -len/2
	
	double thetaInc = M_PI /2;

	for (double p = 0; p < 2* M_PI ; p += thetaInc) {
		for (double d = 0; d < 2 * M_PI; d += thetaInc)
		{
			std::vector<GsVec>   Q;
			Q.push_back(GsVec(cos(d)*sin(p), sin(d)*sin(p), cos(p)));
			Q.push_back(GsVec(cos(d+ thetaInc)*sin(p), sin(d+ thetaInc)*sin(p), cos(p)));
			Q.push_back(GsVec(cos(d)*sin(p + thetaInc), sin(d)*sin(p + thetaInc), cos(p + thetaInc)));
			subdiv(Q, degree);
			
			std::vector<GsVec>   S;
			S.push_back(GsVec(cos(d + thetaInc)*sin(p+thetaInc), sin(d + thetaInc)*sin(p+thetaInc), cos(p+thetaInc)));
			S.push_back(GsVec(cos(d + thetaInc)*sin(p), sin(d + thetaInc)*sin(p), cos(p)));
			S.push_back(GsVec(cos(d)*sin(p + thetaInc), sin(d)*sin(p + thetaInc), cos(p + thetaInc)));
			subdiv(S, degree);

		}
	}
		
	// circle radius rb, vertex at cos, sin, len/2

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 3 * sizeof(float), &C[0], GL_STATIC_DRAW);

	_numpoints = P.size();
	P.resize(0); C.resize(0);
}


void SoCapsule_2 :: subdiv(std::vector<GsVec>& Q, int degree) {
	if (degree == 0) 
	{
		P.push_back(Q[0]);
		P.push_back(Q[1]);
		P.push_back(Q[2]);
		C.push_back(Q[0]);
		C.push_back(Q[1]);
		C.push_back(Q[2]);
	}
	else {
		std::vector<GsVec>   A;
		std::vector<GsVec>   R;
		std::vector<GsVec>   D;
		std::vector<GsVec>   S;
		GsVec mid_1 = (Q[0] + Q[1]) / 2;
		GsVec mid_2 = (Q[1] + Q[2]) / 2;
		GsVec mid_3 = (Q[2] + Q[0]) / 2;
		mid_1.normalize();
		mid_2.normalize();
		mid_3.normalize();

		A.push_back(Q[0]);
		A.push_back(mid_1);
		A.push_back(mid_3);
		subdiv(A, degree - 1);

		R.push_back(Q[1]);
		R.push_back(mid_1);
		R.push_back(mid_2);
		subdiv(R, degree - 1);

		D.push_back(Q[2]);
		D.push_back(mid_2);
		D.push_back(mid_3);
		subdiv(D, degree - 1);

		S.push_back(mid_1);
		S.push_back(mid_2);
		S.push_back(mid_3);
		subdiv(S, degree - 1);

	}


}



void SoCapsule_2::draw(GsMat& tr, GsMat& pr,const light&l, const mat& m) {
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

}

