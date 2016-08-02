#pragma once

#include <TH2D.h>

// make container for the histograms produced by taking the sum og the timediffs for associated layers 
// X, Y detector image hist holders

struct calibrateLayersHist {
	//holds integration along x axis for negative detector
	TH1D* UVMasklayer;
	TH1D* UWMasklayer;
	TH1D* VWMasklayer;
	//holds integration along y axis for negative detector
	TH1D* UVMasklayerY;
	TH1D* UWMasklayerY;
	TH1D* VWMasklayerY;
	//holds integration along x axis for positive detector
	TH1D* UVPosMasklayer;
	TH1D* UWPosMasklayer;
	TH1D* VWPosMasklayer;
	TH1D* UVPosMasklayerY;
	TH1D* UWPosMasklayerY;
	TH1D* VWPosMasklayerY;
};