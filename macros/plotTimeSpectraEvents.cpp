#include "HistogramPair.h"
#include "TTree.h"
#include "TH1D.h"
#include "configuration.h"
#include "DataSet.h"

using namespace std;

HistogramPair plotTimeSpectraEvents(DataSet* data) {

	//Histogram both positive and negative MCP detector pulses, all relative to positron time
	HistogramPair histograms;
	histograms.positive = new TH1D("hpos", "TimeSpectra positive", 200, 1500, 2500);
	histograms.negative = new TH1D("hneg", "TimeSpectra negative", 200, -2000, 32000);


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
	return histograms;
}