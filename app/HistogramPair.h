#ifndef HISTOGRAM_PAIR_H
#define HISTOGRAM_PAIR_H

#include "TH1D.h"

//make container for the 2 histograms (1 for each detector) so they can be stored and returned by the code

struct HistogramPair{
	TH1D* positive;
	TH1D* negative;
};

#endif