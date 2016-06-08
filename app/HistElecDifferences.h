#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct HistogramDIFF {
	TH1D* ydiffuv_uw;
	TH1D* ydiffuv_vw;
	TH1D* xdiffuv_vw;
	TH1D* ydiffuw_vw;
	TH1D* xdiffuw_vw;
}; 
