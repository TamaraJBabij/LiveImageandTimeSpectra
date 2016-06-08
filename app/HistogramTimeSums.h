#pragma once

#include "TH1D.h"
#include <memory>

//make container for the histograms produced by taking the sum of the timediffs for associated layers
// Timesum histograms

struct HistogramTimeSums {
	TH1D* layer_upos;
	//std::unique_ptr<TH1D> layer_upos;
	//std::shared_ptr<TH1D> layer_vpos;
	//std::shared_ptr<TH1D> layer_wpos;
	//std::shared_ptr<TH1D> layer_uneg;
	//std::shared_ptr<TH1D> layer_vneg;
	//std::shared_ptr<TH1D> layer_wneg;
	TH1D* layer_vpos;
	TH1D* layer_wpos;
	TH1D* layer_uneg;
	TH1D* layer_vneg;
	TH1D* layer_wneg;
	//Shared ptr
	//std::shared_ptr<TH1D> test;
};

