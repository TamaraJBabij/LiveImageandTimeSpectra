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

void histogramMaskLayers(DataSet *reconData, calibrateLayersHist* UVWMasklayers) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y

	for (Group* g : *reconData) {
		for (Event* e : g->events) {
			if (e->negative.xy_uv == true) {
				if (e->negative.y_uv<60 && e->negative.y_uv>-60) {
					UVWMasklayers->UVMasklayer->Fill(e->negative.x_uv);
				}
			}
			if (e->negative.xy_uw == true) {
				if (e->negative.y_uw<60 && e->negative.y_uw>-60) {
					UVWMasklayers->UWMasklayer->Fill(e->negative.x_uw);
				}
			}
			if (e->negative.xy_vw == true) {
				if (e->negative.y_vw<60 && e->negative.y_vw> -60) {
					UVWMasklayers->VWMasklayer->Fill(e->negative.x_vw);
				}
			}/*
			if (e->positive.xy_uv == true) {
				if (e->positive.y_uv<60 && e->positive.y_uv>-60) {
					UVWMasklayers->UVPosMasklayer->Fill(e->positive.x_uv);
				}
			}
			if (e->positive.xy_uw == true) {
				if (e->positive.y_uw<60 && e->positive.y_uw>-60) {
					UVWMasklayers->UWPosMasklayer->Fill(e->positive.x_uw);
				}
			}
			if (e->positive.xy_vw == true) {
				if (e->positive.y_vw<60 && e->positive.y_vw> -60) {
					UVWMasklayers->VWPosMasklayer->Fill(e->positive.x_vw);
				}
			}*/
			if (e->positive.xy_uv == true) {
				if (e->positive.x_uv<5 && e->positive.x_uv>-5) {
					UVWMasklayers->UVPosMasklayer->Fill(e->positive.y_uv);
				}
			}
			if (e->positive.xy_uw == true) {
				if (e->positive.x_uw<5 && e->positive.x_uw>-5) {
					UVWMasklayers->UWPosMasklayer->Fill(e->positive.y_uw);
				}
			}
			if (e->positive.xy_vw == true) {
				if (e->positive.x_vw<5 && e->positive.x_vw> -5) {
					UVWMasklayers->VWPosMasklayer->Fill(e->positive.y_vw);
				}
			}
		}
	}
}