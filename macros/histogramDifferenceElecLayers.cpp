#include "stdafx.h"
#include <iostream>
#include <TH1.h>
#include <TF1.h>
#include <TH2D.h>
#include "HistElecDifferences.h"
#include "DataSet.h"
#include "Group.h"

// takes the X,Y positions and makes a 2D histogram for each detector
// hist for positrons, ions and electrons separately

HistogramDIFF histogramElecLayerDiff(DataSet *reconData) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	HistogramDIFF UVWdifferences;
	UVWdifferences.ydiffuv_uw = new TH1D("electronDet", "y uv - uw", 2000, -60, 60);
	UVWdifferences.ydiffuv_vw = new TH1D("electronDET", "y uv - vw", 2000, -60, 60);
	UVWdifferences.xdiffuv_vw = new TH1D("electronDET", "x uv - vw", 2000, -60, 60);
	UVWdifferences.ydiffuw_vw = new TH1D("electronDET", "y uw - vw", 2000, -60, 60);
	UVWdifferences.xdiffuw_vw = new TH1D("electronDET", "x uw - vw", 2000, -60, 60);

	for (Group* g : *reconData) {
		if (g->electron.xy_uv == true && g->electron.xy_uw == true) {
			UVWdifferences.ydiffuv_uw->Fill(g->electron.y_uv_uw);
		}
		if (g->electron.xy_uv == true && g->electron.xy_vw == true) {
			UVWdifferences.ydiffuv_vw->Fill(g->electron.y_uv_vw);
			UVWdifferences.xdiffuv_vw->Fill(g->electron.x_uv_vw);
		}
		if (g->electron.xy_uw == true && g->electron.xy_vw == true) {
			UVWdifferences.ydiffuw_vw->Fill(g->electron.y_uw_vw);
			UVWdifferences.xdiffuw_vw->Fill(g->electron.x_uw_vw);
		}
	} 

	return UVWdifferences;
}