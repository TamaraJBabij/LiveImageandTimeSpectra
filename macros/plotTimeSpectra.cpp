// creats histogram of time of flights (given by MCP signal) of the particles. 
#include <iostream>
#include "HistogramPair.h"
#include "TTree.h"
#include "TH1D.h"
#include "configuration.h"
#include "TFile.h"

using namespace std;


// returns histogram
// return type, function name(argument)
// where DataSet is returned from the tree reader



TH1D* plotTimeSpectra(TTree* tree) {
	//Sets up canvas I guess?

	//setting up to read our tree of processed time data
	Int_t channel;
	Int_t groupNumber;
	Double_t time;
	tree->SetBranchAddress("GroupNumber", &groupNumber);
	tree->SetBranchAddress("Channel", &channel);
	tree->SetBranchAddress("Time", &time);

	//Histogram both positive and negative MCP detector pulses, all relative to positron time
	TH1D *hpos = new TH1D("hpos", "TimeSpectra positive", 200, 0, 32000);
	//TH1D *hneg = new TH1D("hneg", "TimeSpectra negative", 200, -100, 32000);
	//hpos->SetDirectory(0);
	Double_t timeDouble;

	int N = (int)tree->GetEntries();
	cout << N << endl;

	for (int i = 0; i < N; i++) {
		tree->GetEntry(i);
		switch (channel) {
		case CFG_CHANNEL_POS_CP2:
			hpos->Fill(time);
			break;
		case CFG_CHANNEL_ELEC_CP2:
			//hneg->Fill(timeDouble);
			break;
		}
	}
	return hpos;
}
//Draw histograms, which represent time spectra relative to the positron hit
//Want to export histogram data as well as save time spectra figures
//need to set up canvas first

//hist.negative->Draw();