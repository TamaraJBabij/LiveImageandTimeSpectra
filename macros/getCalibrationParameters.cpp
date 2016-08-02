#include "PitchPropSet.h"
#include "DataSet.h"
#include <math.h>
#include "TMatrixDSym.h"
#include "TVector.h"
#include "TArrayD.h"
#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"

double testSolution(PitchPropData s, double Au2, double AuAv, double AuAw, double AuBu, double AuBv, double AuBw, double Au, double Av2, double AvAw, double AvBu, double AvBv, double AvBw, double Av, double Aw2, double AwBu, double AwBv, double AwBw, double Aw, double Bu2, double BuBv, double BuBw, double Bu, double Bv2, double BvBw, double Bv, double Bw2, double Bw, double C) {
	return Au2 * s.uPitchProp * s.uPitchProp +
		AuAv * s.uPitchProp * s.vPitchProp +
		AuAw * s.uPitchProp * s.wPitchProp +
		AuBu * s.uPitchProp * s.uOffset +
		AuBv * s.uPitchProp * s.vOffset +
		AuBw * s.uPitchProp * s.wOffset +
		Au * s.uPitchProp +
		Av2 * s.vPitchProp * s.vPitchProp +
		AvAw * s.vPitchProp * s.wPitchProp +
		AvBu * s.vPitchProp * s.uOffset +
		AvBv * s.vPitchProp * s.vOffset +
		AvBw * s.vPitchProp * s.wOffset +
		Av * s.vPitchProp +
		Aw2 * s.wPitchProp * s.wPitchProp +
		AwBu * s.wPitchProp * s.uOffset +
		AwBv * s.wPitchProp * s.vOffset +
		AwBw * s.wPitchProp * s.wOffset +
		Aw * s.wPitchProp +
		Bu2 * s.uOffset * s.uOffset +
		BuBv * s.uOffset * s.vOffset +
		BuBw * s.uOffset * s.wOffset +
		Bu * s.uOffset +
		Bv2 * s.vOffset * s.vOffset +
		BvBw * s.vOffset * s.wOffset +
		Bv * s.vOffset +
		Bw2 * s.wOffset * s.wOffset +
		Bw * s.wOffset + 
		C;
}

double nearestHole(double pos) {
	return fmax(-35.0, (fmin(35.0, round(pos / 5) * 5)));
}

PitchPropData getCalibrationParameters(DataSet* dataSet, PitchPropSet initial, imagingDetectors userDet) {

	// Coefficients
	double Au2 = 0, AuAv = 0, AuAw = 0, AuBu = 0, AuBv = 0, AuBw = 0, Au = 0;
	double           Av2 = 0, AvAw = 0, AvBu = 0, AvBv = 0, AvBw = 0, Av = 0;
	double                     Aw2 = 0, AwBu = 0, AwBv = 0, AwBw = 0, Aw = 0;
	double                               Bu2 = 0, BuBv = 0, BuBw = 0, Bu = 0;
	double                                         Bv2 = 0, BvBw = 0, Bv = 0;
	double                                                   Bw2 = 0, Bw = 0;
	double C = 0;


	double sqrtThird = sqrt(1.0 / 3);

	for (Group* g : *dataSet) {
		for (Event* e : g->events) {
			Particle p;
			if (userDet == negDet) {
				p = e->negative;
			}
			else {
				p = e->positive;
			}

			if (p.xy_uv) {
				double x = nearestHole(p.x_uv);
				double y = nearestHole(p.y_uv);
				double u = e->uPairs.front().line1 - e->uPairs.front().line2;
				double v = e->vPairs.front().line1 - e->vPairs.front().line2;

				// Coeffs for x^2
				Au2 += u * u;
				AuBu += 2 * u;
				Au -= 2 * x * u;

				Bu2 += 1;
				Bu -= 2 * x;

				// Coeffs for y^2
				Av2 += (4.0 / 3) * v * v;
				AvBv += 2 * (4.0 / 3) * v;
				AuAv -= 2 * (2.0 / 3) * v * u;
				AvBu -= 2 * (2.0 / 3) * v;
				Av -= 2 * 2 * sqrtThird * v * y;

				Bv2 += (4.0 / 3);
				AuBv -= 2 * (2.0 / 3) * u;
				BuBv -= 2 * (2.0 / 3);
				Bv -= 2 * 2 * sqrtThird * y;

				Au2 += (1.0 / 3) * u * u;
				AuBu += 2 * (1.0 / 3) * u;
				Au += 2 * sqrtThird * u * y;

				Bu2 += (1.0 / 3);
				Bu += 2 * sqrtThird * y;

				C += x * x + y * y;
			}

			if (p.xy_uw) {
				double x = nearestHole(p.x_uw);
				double y = nearestHole(p.y_uw);
				double u = e->uPairs.front().line1 - e->uPairs.front().line2;
				double w = e->wPairs.front().line1 - e->wPairs.front().line2;

				// Coeffs for x^2
				Au2 += u * u;
				AuBu += 2 * u;
				Au -= 2 * x * u;

				Bu2 += 1;
				Bu -= 2 * x;

				// Coeffs for y^2
				Aw2 += (4.0 / 3) * w * w;
				AwBw += 2 * (4.0 / 3) * w;
				AuAw += 2 * (2.0 / 3) * w * u;
				AwBu += 2 * (2.0 / 3) * w;
				Aw -= 2 * 2 * sqrtThird * w * y;

				Bw2 += (4.0 / 3);
				AuBw += 2 * (2.0 / 3) * u;
				BuBw += 2 * (2.0 / 3);
				Bw -= 2 * 2 * sqrtThird * y;

				Au2 += (1.0 / 3) * u * u;
				AuBu += 2 * (1.0 / 3) * u;
				Au -= 2 * sqrtThird * u * y;

				Bu2 += (1.0 / 3);
				Bu -= 2 * sqrtThird * y;
				C += x * x + y * y;
			}

			if (p.xy_vw) {
				double x = nearestHole(p.x_vw);
				double y = nearestHole(p.y_vw);
				double v = e->vPairs.front().line1 - e->vPairs.front().line2;
				double w = e->wPairs.front().line1 - e->wPairs.front().line2;

				// Coeffs for x^2
				Av2 += v * v;
				AvBv += 2 * v;
				AvAw -= 2 * v * w;
				AvBw -= 2 * v;
				Av -= 2 * v * x;

				Bv2 += 1;
				AwBv -= 2 * w;
				BvBw -= 2;
				Bv -= 2 * x;

				Aw2 += w * w;
				AwBw += 2 * w;
				Aw += 2 * x * w;

				Bw2 += 1;
				Bw += 2 * x;


				// Coeffs for y^2
				Aw2 += (1.0 / 3) * w * w;
				AwBw += 2 * (1.0 / 3) * w;
				AvAw += 2 * (1.0 / 3) * w * v;
				AwBv += 2 * (1.0 / 3) * w;
				Aw -= 2 * sqrtThird * w * y;

				Bw2 += (1.0 / 3);
				AvBw += 2 * (1.0 / 3) * v;
				BvBw += 2 * (1.0 / 3);
				Bw -= 2 * sqrtThird * y;

				Av2 += (1.0 / 3) * v * v;
				AvBv += 2 * (1.0 / 3) * v;
				Av -= 2 * sqrtThird * v * y;

				Bv2 += (1.0 / 3);
				Bv -= 2 * sqrtThird * y;
				C += x * x + y * y;
			}
		}
	}
	PitchPropData negPitches = initial.getPitchProp(negative);

	cout << "Initial SSE = " << testSolution(negPitches, Au2, AuAv, AuAw, AuBu, AuBv, AuBw, Au, Av2, AvAw, AvBu, AvBv, AvBw, Av, Aw2, AwBu, AwBv, AwBw, Aw, Bu2, BuBv, BuBw, Bu, Bv2, BvBw, Bv, Bw2, Bw, C) << endl;

	double mat[6*6] = {
		2 * Au2, AuAv, AuAw, AuBu, AuBv, AuBw,
		AuAv, 2 * Av2, AvAw, AvBu, AvBv, AvBw,
		AuAw, AvAw, 2 * Aw2, AwBu, AwBv, AwBw,
		AuBu, AvBu, AwBu, 2 * Bu2, BuBv, BuBw,
		AuBv, AvBv, AwBv, BuBv, 2 * Bv2, BvBw,
		AuBw, AvBw, AwBw, BuBw, BvBw, 2 * Bw2
	};

	double vec[6] = { -Au, -Av, -Aw, -Bu, -Bv, -Bw };

	TMatrixDSym coefficientMatrix(6, mat);
	TVectorD coefficientVector(6, vec);
	TMatrixDSym inverse = coefficientMatrix.Invert();
	coefficientVector *= inverse;

	PitchPropData result;
	result.uPitchProp = coefficientVector[0]*2.00;
	result.vPitchProp = coefficientVector[1]*2.00;
	result.wPitchProp = coefficientVector[2]*2.00;
	result.uOffset = coefficientVector[3];
	result.vOffset = coefficientVector[4];
	result.wOffset = coefficientVector[5];


	cout << "Final SSE = " << testSolution(result, Au2, AuAv, AuAw, AuBu, AuBv, AuBw, Au, Av2, AvAw, AvBu, AvBv, AvBw, Av, Aw2, AwBu, AwBv, AwBw, Aw, Bu2, BuBv, BuBw, Bu, Bv2, BvBw, Bv, Bw2, Bw, C) << endl;

	return result;
}