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

void histogramElectronLayers(DataSet *reconData, HistogramElecLayers * UVWlayers, imagingDetectors userDet) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y

	for (Group* g : *reconData) {
		for (Event* e : g->events) {
			if (e->negative.xy_uv == true) {
				UVWlayers->UVlayers->Fill(e->negative.x_uv, e->negative.y_uv);
			}
			if (e->negative.xy_uw == true) {
				UVWlayers->UWlayers->Fill(e->negative.x_uw, e->negative.y_uw);
			}
			if (e->negative.xy_vw == true) {
				UVWlayers->VWlayers->Fill(e->negative.x_vw, e->negative.y_vw);
			}
			if (e->positive.xy_uv == true) {
				UVWlayers->UVPoslayers->Fill(e->positive.x_uv, e->positive.y_uv);
			}
			if (e->positive.xy_uw == true) {
				UVWlayers->UWPoslayers->Fill(e->positive.x_uw, e->positive.y_uw);
			}
			if (e->positive.xy_vw == true) {
				UVWlayers->VWPoslayers->Fill(e->positive.x_vw, e->positive.y_vw);
			}
		}
	}
}