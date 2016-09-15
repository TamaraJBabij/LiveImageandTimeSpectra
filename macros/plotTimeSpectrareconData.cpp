#include "HistogramPair.h"
#include "TTree.h"
#include "TH1D.h"
#include "configuration.h"
#include "DataSet.h"
#include "Constants.h"
#include <iostream>
#include "DataSet.h"
#include "stdafx.h"
#include "Group.h"
#include "PitchPropSet.h"
#include "PitchData.h"
#include "math.h"
#include "Constants.h"
#include "histogramElecLayers.h"
#include "calibrateLayersHist.h"

using namespace std;

void plotTimeSpectraRadius(DataSet* data, imagingDetectors userDet, HistogramPair *histograms) {
	//Plots the time of every MCP (CP2) hit
	//This is in absolute timing, as the events have not been made relative to the positron hit
	for (Group* g : *data) {
		for (Event* e : g->events) {
			if (e->mcp->detector == pos && (userDet == posDet || userDet == bothDet)) {
				double r = sqrt(pow(e->positive.x, 2.0) + pow(e->positive.y, 2.0));
				if (r < 40.0) {
					histograms->positive->Fill(e->mcp->time);
				}

			}
			else if (e->mcp->detector == neg && (userDet == negDet || userDet == bothDet)) {
				histograms->negative->Fill(e->mcp->time);

			}


		}
	}
}
