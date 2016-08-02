#pragma once
#include "stdafx.h"
#include <TH1D.h>

// Container for layer sum histograms

struct layerDiffHist {

	//layer Sums, ex: u1+u2, v1+v2 etc
	TH1D* uPosLayerDiff;
	TH1D* vPosLayerDiff;
	TH1D* wPosLayerDiff;
	TH1D* uNegLayerDiff;
	TH1D* vNegLayerDiff;
	TH1D* wNegLayerDiff;
};