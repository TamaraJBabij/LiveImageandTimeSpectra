#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct HistogramElecLayers {
	TH2D* UVlayers;
	TH2D* UWlayers;
	TH2D* VWlayers;
	TH2D* UVPoslayers;
	TH2D* UWPoslayers;
	TH2D* VWPoslayers;
};