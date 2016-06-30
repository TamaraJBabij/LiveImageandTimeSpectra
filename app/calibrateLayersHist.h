#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct calibrateLayersHist {
	TH1D* UVMasklayer;
	TH1D* UWMasklayer;
	TH1D* VWMasklayer;
};