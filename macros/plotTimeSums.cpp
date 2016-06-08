#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Event.h"
#include "stdafx.h"
#include "HistogramTimeSums.h"
#include "Hit.h"
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TH1D.h>
#include "TObject.h"
#include "TFile.h"

using namespace std;

// program plots time sums and fits a gaussian 
// gets parameters for fit and parses them for time sum checking

void plotTimeSums(HistogramTimeSums timesums) {

	TCanvas c2("c2", "Second Canvas");
	//set up canvas for time sums - 3 for each detector - 6 in total
	//TPad::Divide() specifies number of vertical and horizontal slices of canvas
	c2.Divide(2, 3);
	c2.cd(1);
	timesums.layer_upos->Draw();
	timesums.layer_upos->Fit("gaus");
	c2.cd(2);
	timesums.layer_vpos->Draw();
	timesums.layer_vpos->Fit("gaus");
	c2.cd(3);
	timesums.layer_wpos->Draw();
	timesums.layer_wpos->Fit("gaus");
	c2.cd(4);
	timesums.layer_uneg->Draw();
	timesums.layer_uneg->Fit("gaus");
	c2.cd(5);
	timesums.layer_vneg->Draw();
	timesums.layer_vneg->Fit("gaus");
	c2.cd(6);
	timesums.layer_wneg->Draw();
	timesums.layer_wneg->Fit("gaus");
	//rootapp->Run();


}