#pragma once

#include <TH2D.h>

// Container for histograms of the differences of specific layer points

struct histSumOfDiff {
	TH2D* xdiffuv_vw;
	TH2D* ydiffuv_uw_vw;
};

