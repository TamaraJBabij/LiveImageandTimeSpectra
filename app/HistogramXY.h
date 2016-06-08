#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct HistogramXY {
	TH2D* positronDET;
	TH2D* electronDET;
	TH2D* ionDET;
};