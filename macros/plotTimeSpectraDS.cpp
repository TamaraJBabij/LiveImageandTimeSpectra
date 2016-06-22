#include "HistogramPair.h"
#include "TTree.h"
#include "TH1D.h"
#include "configuration.h"
#include "DataSet.h"
#include "Constants.h"

using namespace std;

HistogramPair plotTimeSpectraDS(DataSet* data, imagingDetectors userDet) {

	//Histogram both positive and negative MCP detector pulses, all relative to positron time
	HistogramPair histograms;
	histograms.positive = new TH1D("hpos", "TimeSpectrum positive", 500, -2000, 32000);
	histograms.negative = new TH1D("hneg", "TimeSpectrum negative", 500, -2000, 32000);

	if (userDet == bothDet) {
		for (Group* g : *data) {
			for (Hit* h : *g) {
				if (h->channel == mcp) {
					if (h->detector == pos) {
						histograms.positive->Fill(h->time);
					}
					else if (h->detector == neg) {
						histograms.negative->Fill(h->time);
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
							histograms.positive->Fill(h->time);
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
							histograms.negative->Fill(h->time);
						}

					}
				}
			}
		
	}
	return histograms;
}