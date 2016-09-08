
# include "curve_eval.h"

CurveEval::CurveEval() {
	_numpoints = 0;
}

void CurveEval::init(const GlProgram& prog) {
	// Define buffers needed:
	set_program(prog);    // use base class GlObjects to keep program id
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


void CurveEval::build(float r) {

	// send data to OpenGL buffers:

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // break the existing vertex array object binding.

						  // save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0); N.resize(0);
}

// draw will be called everytime we need to display this object:
void CurveEval::draw(GsMat& tr, GsMat& pr, const light& l, const mat & m)
{
	// Prepare program:
	glUseProgram(GlObjects::prog);
	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	// Draw:
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; // we convert below our color values to be in [0,1]   
	glUniform3fv(uniloc[2], 1, l.pos.e);
	l.amb.get(f);
	glUniform4fv(uniloc[3], 1, f);
	l.dif.get(f);
	glUniform4fv(uniloc[4], 1, f);
	l.spe.get(f);
	glUniform4fv(uniloc[5], 1, f);
	m.amb.get(f);
	glUniform4fv(uniloc[6], 1, f);
	m.dif.get(f);
	glUniform4fv(uniloc[7], 1, f);
	m.spe.get(f);
	glUniform4fv(uniloc[8], 1, f);
	glUniform1fv(uniloc[9], 1, &m.shine);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}



GsArray<GsVec> CurveEval::eval_bezier(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {

	Q.capacity(0);
	if (t < 0.001) {
		for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			Q.push(ctrlpnts.get(i));
		}
	}
	else if (t > 0.999) {
		Q.push(ctrlpnts.get(0));
		for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			Q.push(ctrlpnts.get(i + 1));
		}
	} else {
		Q.push(ctrlpnts.get(0));
		for (int n = 0; n < ctrlpnts.size() - 1; n++) {
			subdivBez(t, ctrlpnts.get(n), ctrlpnts.get(n + 1), ctrlpnts, numRecursions);
		}
		Q.push(ctrlpnts.get(ctrlpnts.size() - 1));
	}
	return Q;
}

void CurveEval::subdivBez(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	GsVec legLen = t * (b - a);
	legLen += a;
	if (numRecursions > 0) {
		subdivBez(t, a, legLen, ctrlpnts, (numRecursions - 1));
	}
	Q.push(legLen);
	if (numRecursions > 0) {
		subdivBez(t, legLen, b, ctrlpnts, (numRecursions - 1));
	}
}

GsArray<GsVec> CurveEval::eval_lagrange(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {

	GsArray<GsVec> R;
	R.capacity(0);
	S.capacity(0);
	//R.push(ctrlpnts.get(0));
	float y = 1, nextY = .0f;
	//std::cout << "size: " << ctrlpnts.size() << "\n";
	for (int i = 0; i < ctrlpnts.size() - 1; i++) {
		subdivGrange(t, ctrlpnts.get(i), ctrlpnts.get(i + 1), ctrlpnts, numRecursions);
	}
	
	return S;
}
void CurveEval::subdivGrange(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numIt) {
	float y = 1, nextY = .0f;
	float x = a.x + t * (b.x - a.x);
	for (int j = 0; j < ctrlpnts.size(); j++) {
		//y = t * (ctrlpnts.get(j).y - ctrlpnts.get(j + 1).y);
		for (int k = 0; k < ctrlpnts.size(); k++) {
			if (j == k) {
				y *= ctrlpnts.get(j).y;
			}
			else {
				y *= (x - ctrlpnts.get(k).x) / (ctrlpnts.get(j).x - ctrlpnts.get(k).x);
			}
		}
		nextY += y;
		y = 1;
	}
	if (numIt == 0) {
		if (a.y != 0) {
			S.push(a);
		}
	}
	else {
		subdivGrange(t, a, GsVec(x, nextY, .0f), ctrlpnts, (numIt - 1));
	}
	
	S.push(GsVec(x, nextY, .0f));
	nextY = .0f;

	if (numIt == 0) {
		if (b.y != 0) {
			S.push(b);
		}
	}
	else {
		subdivGrange(t, GsVec(x, nextY, .0f), b, ctrlpnts, (numIt - 1));
	}
}


GsArray<GsVec> CurveEval::eval_bspline(float t, int k, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	B.capacity(0);
	int n = ctrlpnts.size() - 1;
	GsVec newpoint;
	float increment = t;
	increment /= (numRecursions + 1);
	for (float point = 2; point <= n + 1; point += increment) {
		newpoint =  GsVec(0.0, 0.0, 0.0);
		float sum = 0;
		for (int i = 0; i < n + 1; i++)
		{
			newpoint += ctrlpnts(i)*subdivBSpline(point, i, 3);
		}
		B.push(newpoint);
	}
	return B;
}

float CurveEval::subdivBSpline(float location, int currentloc, int degree) {
	float ui = currentloc;//current location on the line(the point we are currently doing)[0,1,2,3,ect...]
	float u = location;

	if (degree == 1)
	{
		if (ui <= u && u < ui + 1)
			return 1.0f;//changed from void return
		else {
			return 0;
		}
	}
	else {
		return (((u - ui) / (degree - 1))*subdivBSpline(location, int(currentloc), degree - 1) + ((ui + degree - u) / (degree - 1))*subdivBSpline(location, int(currentloc + 1), degree - 1));
	}
}

GsArray<GsVec> CurveEval::eval_crspline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	int n = ctrlpnts.size() - 1;
	CR.capacity(0);
	//CR.push(ctrlpnts.get(0));
	GsArray<GsVec> temp = ctrlpnts, temp2;
		for (int i = 1; i < temp.size() - 1; i++) {
			temp2.push(subdivCRSpline(t, i, temp, numRecursions));
		}
	CR.push(temp2);
	CR.remove(0);
	CR.remove(CR.size() - 1);

	return CR;
}

GsArray<GsVec> CurveEval::subdivCRSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	GsVec Ip = (ctrlpnts[p + 1] - ctrlpnts[p - 1]) / 2;
	GsArray<GsVec> newCR;
	newCR.push(ctrlpnts[p] - (Ip / 3));
	newCR.push(ctrlpnts[p]);
	newCR.push(ctrlpnts[p] + (Ip / 3));
	return newCR;
}


GsArray<GsVec> CurveEval::eval_bospline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	BO.capacity(0);
	int n = ctrlpnts.size() - 1;
	GsArray<GsVec> temp;
	GsVec u0, u1, u2, v0, v1;
	float d0, d1;
	for (int p = 0; p < ctrlpnts.size(); p++) {
		u1 = ctrlpnts[p];
		u1.normalize();
		if (p > 0) {
			u0 = ctrlpnts[p - 1];
			u0.normalize();
			d0 = sqrt((ctrlpnts[p] - ctrlpnts[p - 1]).norm2());
			v0 = (ctrlpnts[p] - ctrlpnts[p - 1]) / d0;
		}
		else {
			d0 = 0;
		}
		if (p < ctrlpnts.size() - 1) {
			u2 = ctrlpnts[p + 1];
			u2.normalize();
			d1 = sqrt((ctrlpnts[p + 1] - ctrlpnts[p]).norm2());
			v1 = (ctrlpnts[p + 1] - ctrlpnts[p]) / d1;
		}
		else {
			d1 = 0;
		}
		GsVec vPrime = ((d1 * v0) + (d0 * v1)) / (d1 + d0);
		if (p > 0 && p < ctrlpnts.size() - 1) {
			temp.push(ctrlpnts.get(p) - (vPrime * d0 / 3));

			temp.push(ctrlpnts.get(p));
			temp.push(ctrlpnts.get(p) + (vPrime * d1 / 3));
		}
	}
	temp.remove(0);
	temp.remove(temp.size() - 1);
	BO = temp;
	return BO;
}
GsArray<GsVec> CurveEval::subdivBOSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	GsArray<GsVec> temp;
	GsVec u0, u1, u2, v0, v1;
	float d0, d1;
	u1 = ctrlpnts[p];
	u1.normalize();
	if (p > 0) {
		u0 = ctrlpnts[p - 1];
		u0.normalize();
		d0 = sqrt((ctrlpnts[p] - ctrlpnts[p - 1]).norm2());
		v0 = (ctrlpnts[p] - ctrlpnts[p - 1]) / d0;
	} else {
		d0 = 0;
	}
	if (p < ctrlpnts.size() - 1) {
		u2 = ctrlpnts[p + 1];
		u2.normalize();
		d1 = sqrt((ctrlpnts[p + 1] - ctrlpnts[p]).norm2());
		v1 = (ctrlpnts[p + 1] - ctrlpnts[p]) / d1;
	} else {
		d1 = 0;
	}
	GsVec vPrime = ((d1 * v0) + (d0 * v1)) / (d1 + d0);
	if (p > 0 && p < ctrlpnts.size() - 1) {
		temp.push(ctrlpnts.get(p) - (vPrime * d0 / 3));
		temp.push(ctrlpnts.get(p));
		temp.push(ctrlpnts.get(p) + (vPrime * d1 / 3));
	}
	return temp;
}

GsArray<GsVec> CurveEval::setGuide(float t, const GsArray<GsVec> &ctrlpnts, bool smoothShading) {
	P.capacity(0); C.capacity(0); N.resize(0);
	
	for (int i = 0; i < ctrlpnts.size() - 1; i++) {
		P.push(ctrlpnts.get(i));	C.push(GsColor::cyan);
		GsVec n = ctrlpnts.get(i);
		n.normalize();
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1));	C.push(GsColor::cyan);
		GsVec o = GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1);
		o.normalize();
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::cyan);
		GsVec m = ctrlpnts.get(i + 1);
		m.normalize();
		if (smoothShading) {
			N.push_back(n);
			N.push_back(o);
			N.push_back(m);
		}
		else {
			N.push_back(n);
			N.push_back(n);
			N.push_back(n);
		}
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::magenta);
		P.push(GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, -1));	C.push(GsColor::magenta);
		n = GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, -1);
		n.normalize();
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1));	C.push(GsColor::magenta);
		o = GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1);
		o.normalize();
		if (smoothShading) {
			N.push_back(m);
			N.push_back(n);
			N.push_back(o);
		}
		else {
			N.push_back(m);
			N.push_back(m);
			N.push_back(m);
		}
	}
	return Q;
}