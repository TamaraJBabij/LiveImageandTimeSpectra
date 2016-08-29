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

void plotTimeSpectraDS(DataSet* data, imagingDetectors userDet, HistogramPair *histograms) {
	//Plots the time of every MCP (CP2) hit
	//This is in absolute timing, as the events have not been made relative to the positron hit
	for (Group* g : *data) {
		for (Hit* h : *g) {
			if (h->channel == mcp) {
				if (h->detector == pos && (userDet == posDet || userDet == bothDet)) {
					histograms->positive->Fill(h->time);
				}
				else if (h->detector == neg && (userDet == negDet || userDet == bothDet)) {
					histograms->negative->Fill(h->time);
				}

			}
		}
	}
}
