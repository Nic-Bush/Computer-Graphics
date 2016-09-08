
# include "curve_eval.h"

CurveEval::CurveEval() {
	_numpoints = 0;
}

void CurveEval::init(const GlProgram& prog) {
	// Define buffers needed:
	set_program(prog);    // use base class GlObjects to keep program id
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}


void CurveEval::build(float r) {

	// send data to OpenGL buffers:

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // break the existing vertex array object binding.

						  // save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0);
}

// draw will be called everytime we need to display this object:
void CurveEval::draw(GsMat& tr, GsMat& pr)
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
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}



GsArray<GsVec> CurveEval::eval_bezier(float t, const GsArray<GsVec> &ctrlpnts) {

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
		for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			GsVec legLen = ctrlpnts.get(i + 1) - ctrlpnts.get(i);
			Q.push(ctrlpnts.get(i) + legLen * t);
		}
		Q.push(ctrlpnts.get(ctrlpnts.size() - 1));
	}
	return Q;
}

GsArray<GsVec> CurveEval::eval_lagrange(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {

	GsArray<GsVec> R;
	R.capacity(0);
	R.push(ctrlpnts.get(0));
	float y = 1, nextY = .0f;
	std::cout << "size: " << ctrlpnts.size() << "\n";
	for (int i = 0; i < numRecursions *  (ctrlpnts.size() - 1); i++) {
		float x = ctrlpnts.get(i / numRecursions).x;
		for (int m = 0; m < numRecursions; m++) {
			x = ctrlpnts.get(i / numRecursions).x + (t - pow(t, m)) * (ctrlpnts.get((i + 1) / numRecursions).x - ctrlpnts.get(i / numRecursions).x);
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
				std::cout << "nextY: " << nextY << "; ";
				y = 1;
			}
			// = t * (ctrlpnts.get(i + 1).y - ctrlpnts.get(i).y);
			R.push(GsVec(x, nextY, .0f));
			nextY = .0f;

			R.push(GsVec(ctrlpnts.get((i + 1) / numRecursions).x, ctrlpnts.get((i + 1) / numRecursions).y, .0f));


			x = ctrlpnts.get(i / numRecursions).x + (t + pow(t, m)) * (ctrlpnts.get((i + 1) / numRecursions).x - ctrlpnts.get(i / numRecursions).x);
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
				std::cout << "nextY: " << nextY << "; ";
				y = 1;
			}
			// = t * (ctrlpnts.get(i + 1).y - ctrlpnts.get(i).y);
			R.push(GsVec(x, nextY, .0f));
			nextY = .0f;

			R.push(GsVec(ctrlpnts.get((i + 1) / numRecursions).x, ctrlpnts.get((i + 1) / numRecursions).y, .0f));
		}
	}
	R.push(ctrlpnts.get(ctrlpnts.size() - 1));

	S.capacity(0);
	S = R;
	return R;
}

GsArray<GsVec> CurveEval::setGuide(float t, const GsArray<GsVec> &ctrlpnts) {
	P.capacity(0); C.capacity(0);
	//setGuide(t, ctrlpnts);
	for (int i = 0; i < ctrlpnts.size() - 1; i++) {
		P.push(ctrlpnts.get(i));	C.push(GsColor::cyan);
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1));	C.push(GsColor::cyan);
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::cyan);
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::magenta);
		P.push(GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, -1));	C.push(GsColor::magenta);
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1));	C.push(GsColor::magenta);
	}
	return Q;
}