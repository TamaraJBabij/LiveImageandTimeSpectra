#pragma once
#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct UVWpositionsHist {
	TH1D* UNegPositions;
	TH1D* VNegPositions;
	TH1D* WNegPositions;
	TH1D* UPosPositions;
	TH1D* VPosPositions;
	TH1D* WPosPositions;
};