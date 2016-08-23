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
	//cout << "plotTimeSpectraDS has run" << endl;
	if (userDet == bothDet) {
		for (Group* g : *data) {
			for (Hit* h : *g) {
				if (h->channel == mcp) {
					if (h->detector == pos) {
						histograms->positive->Fill(h->time);
					}
					else if (h->detector == neg) {
						histograms->negative->Fill(h->time);
					}

				}
			}
		}
	}

	else if (userDet == posDet) {
		
			for (Group* g : *data) {
				for (Hit* h : *g) {
					if (h->channel == mcp) {
						if (h->detector == pos) {
							histograms->positive->Fill(h->time);
						}
					}
				}
			}
		
	}

	//must be negative det only then
	else {
		
			for (Group* g : *data) {
				for (Hit* h : *g) {
					if (h->channel == mcp) {
						if (h->detector == neg) {
							histograms->negative->Fill(h->time);
						}

					}
				}
			}
		
	}
}