// creats histogram of time of flights (given by MCP signal) of the particles. 
#include <iostream>
#include "TH1D.h"
#include "TTree.h"
#include <vector>



TH1D* test() {
	//Histogram both positive and negative MCP detector pulses, all relative to positron time
	TH1D *hpos = new TH1D("hpos", "TimeSpectra positive", 200, 0, 32000);

	hpos->SetDirectory(0);

	for (int i = 0; i < 1000; i++) {
		hpos->Fill(i*i);
	}
	return hpos;
}

TTree* clone(TTree* in) {
	TTree* copy = in->CloneTree();
	return copy;
}