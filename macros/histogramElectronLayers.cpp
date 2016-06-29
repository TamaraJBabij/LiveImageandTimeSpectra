#include "stdafx.h"
#include <iostream>
#include <TH1.h>
#include <TF1.h>
#include <TH2D.h>
#include "histogramElecLayers.h"
#include "DataSet.h"
#include "Group.h"

// takes the X,Y positions and makes a 2D histogram for each detector
// hist for positrons, ions and electrons separately

HistogramElecLayers histogramElectronLayers(DataSet *reconData) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	HistogramElecLayers UVWlayers;
	UVWlayers.UVlayers = new TH2D("electronDet", "UV layer", 2000, -60, 60, 2000, -60, 60);
	UVWlayers.UWlayers = new TH2D("electronDET", "UW layer", 2000, -60, 60, 2000, -60, 60);
	UVWlayers.VWlayers = new TH2D("electronDET", "VW layer", 2000, -60, 60, 2000, -60, 60);

	for (Group* g : *reconData) {
		if (g->negative.xy_uv == true) {
			UVWlayers.UVlayers->Fill(g->negative.x_uv, g->negative.y_uv);
		}
		if (g->negative.xy_uw == true) {
			UVWlayers.UWlayers->Fill(g->negative.x_uw, g->negative.y_uw);
		}
		if (g->negative.xy_vw == true) {
			UVWlayers.VWlayers->Fill(g->negative.x_vw, g->negative.y_vw);
		}
	}

	return UVWlayers;
}