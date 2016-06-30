#include "stdafx.h"
#include <iostream>
#include <TH1.h>
#include <TF1.h>
#include <TH2D.h>
#include "calibrateLayersHist.h"
#include "DataSet.h"
#include "Group.h"

// takes the X,Y positions and makes a 2D histogram for each detector
// hist for positrons, ions and electrons separately

calibrateLayersHist histogramMaskLayers(DataSet *reconData) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	calibrateLayersHist UVWMasklayers;
	UVWMasklayers.UVMasklayer = new TH1D("electronDet", "UV layer", 400, -60, 60);
	UVWMasklayers.UWMasklayer = new TH1D("electronDET", "UW layer", 400, -60, 60);
	UVWMasklayers.VWMasklayer = new TH1D("electronDET", "VW layer", 400, -60, 60);

	for (Group* g : *reconData) {
		if (g->negative.xy_uv == true) {
			if (g->negative.y_uv<2.5 && g->negative.y_uv>-2.5) {
				UVWMasklayers.UVMasklayer->Fill(g->negative.x_uv);
			}	
		}
		if (g->negative.xy_uw == true) {
			if (g->negative.y_uw<2.5 && g->negative.y_uw>-2.5) {
				UVWMasklayers.UWMasklayer->Fill(g->negative.x_uw);
			}	
		}
		if (g->negative.xy_vw == true) {
			if (g->negative.y_vw<2.5 && g->negative.y_vw> -2.5) {
				UVWMasklayers.VWMasklayer->Fill(g->negative.x_vw);
			}	
		}
	}

	return UVWMasklayers;
}