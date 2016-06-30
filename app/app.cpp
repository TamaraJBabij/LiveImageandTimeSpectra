// app.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "macros.h"
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TH1D.h>
#include <TFormula.h>
#include <TH1.h>
#include <TF1.h>
#include <TStyle.h>
#include "DataSet.h"
#include "TFile.h"
#include "HistogramPair.h"
#include "TObject.h"
#include "Event.h"
#include "HistogramTimeSums.h"
#include "HistogramXY.h"
#include "configlayers.h"
#include "Constants.h"
#include "FitSet.h"
#include <TStyle.h>
#include "histogramElecLayers.h"
#include "histogramElecLayers.h"
#include <TGraph.h>
#include <TLegend.h>

using namespace std;

//this will eventually take data from the DAQ software Dan Murtagh wrote
//During testing the program will take in folders like the main analysis software
//This programs purpose is to provide live updating images of the detectors as well as time spectra
//sould be two options 1. Find TS fits and save to tree for later use in program and/or 2. create one or two detector images and time spectra
//this program will not select out for particles and will use all reconstructable hits to produce an image
//

int main(int argc, char* argv[]) {
	//Initial function to set up the debug environment
	int setUpDebugEnvironment();
	//initialises root app
	TApplication* rootapp = new TApplication("example", &argc, argv);

	DataSet* data = scanFiles();

	//In some cases may want to only image one detector, this will save processing time by skipping 
	// not needed detector


	string detinput;

	cout << "what detectors do you want to image: neg, pos or both? " << endl;
	cin >> detinput;

	imagingDetectors userDet;
	if (detinput.compare("pos") == 0) {
		userDet = posDet;
	}
	else if (detinput.compare("neg") == 0) {
		userDet = negDet;
	}
	else if (detinput.compare("both") == 0) {
		userDet = bothDet;
	}
	else {
		cout << "Input does not match a valid detector" << endl;
		exit(1);
	}
	//cout << &userDet << endl;

	/*
	switch (detinput) {
	case "pos":
		imagingDetectors userDet = posDet;
	case "neg":
		imagingDetectors userDet = negDet;
	case "both":
		imagingDetectors userDet = bothDet;
	}
	*/
	
	/* if (detinput == 'pos') {
		userDet = posDet;
		cout << 'pos' << endl;
	}
	else if (detinput == 'neg') {
		userDet = negDet;
		cout << 'neg' << endl;
	}
	else {
		userDet = bothDet;
		cout << 'both' << endl;
	} 
	*/

	//converts the input string to an enumeration
	//imagingDetectors userDet = (imagingDetectors)detinput;

	// implement this later
	//cout << "would you like a layer image: yes, no?" <<

	//tree->Draw("Time");
	HistogramPair hists = plotTimeSpectraDS(data, userDet);

	// need to switch between canvases in root in order to creat two histograms
	//any instance of TCanvas can be subdivided into independent graphical areas, 
	//called pads (by default, a canvas contain a single pad, occupying the whole space - TCanvas inherits from TPad).
	TCanvas c1("c1", "First Canvas");
	if (userDet == bothDet) {
		//TPad::Divide() specifies number of vertical and horizontal slices of canvas
		c1.Divide(1, 2);
		c1.cd(1);
		hists.positive->Draw();
		//hists.positive->Write();
		//Just defualt draw hist option offers irrelevant info for time spectrum
		//gStyle->SetOptStat("");
		c1.cd(2);
		//TFile f("histos.root", "new");
		hists.negative->Draw();
		//hists.negative->Write();
	}
	else if (userDet == posDet) {
		hists.positive->Draw();
	}
	else {
		hists.negative->Draw();
	}

	//Associate hits into events, where event is a single particle/ion hit on the detector. Events are sorted by group
	constructEvents(data);

	// construct timesum histograms
	HistogramTimeSums timesums = calculateTimeSums(data, userDet);

	//Store fit parameters into a tree for later accessing
	// setting up tree
	TFile file("TimeSumTree.root", "recreate");
	TTree treeTS("TimeSumPeaks", "simple tree that stores timesum peaks and sigma");
	//Cant store strings in tree?
	Double_t peak, sigma, error;
	//Will define layer numbers in configuration.h file
	//not sure if text/string can be stored in trees
	Double_t layer;
	treeTS.Branch("Layer", &layer);
	treeTS.Branch("Peak", &peak);
	treeTS.Branch("Sigma", &sigma);
	treeTS.Branch("Error", &error);

	TCanvas c2("c2", "Second Canvas");
	FitSet fits;

	if (userDet == bothDet) {

		//set up canvas for time sums - 3 for each detector - 6 in total
		//TPad::Divide() specifies number of vertical and horizontal slices of canvas
		c2.Divide(2, 3);
	
		c2.cd(1);
		timesums.layer_upos->Draw();
		timesums.layer_upos->Fit("gaus");
		gStyle->SetOptFit(0011);
		TF1 *fitu = timesums.layer_upos->GetFunction("gaus");
		layer = CFG_LAYER_UPOS;
		peak = fitu->GetParameter(1);
		sigma = fitu->GetParameter(2);
		error = fitu->GetParError(1);
		treeTS.Fill();
		fits.setFit(u, positive, peak, error, sigma);
		//Want to display fit parameters on timesums plots
		//timesums.layer_upos->SetOptFit();

		c2.cd(2);
		timesums.layer_vpos->Draw();
		timesums.layer_vpos->Fit("gaus");
		TF1 *fitv = timesums.layer_vpos->GetFunction("gaus");
		layer = CFG_LAYER_VPOS;
		peak = fitv->GetParameter(1);
		sigma = fitv->GetParameter(2);
		error = fitv->GetParError(1);
		treeTS.Fill();
		fits.setFit(v, positive, peak, error, sigma);

		c2.cd(3);
		timesums.layer_wpos->Draw();
		timesums.layer_wpos->Fit("gaus");
		TF1 *fitw = timesums.layer_wpos->GetFunction("gaus");
		layer = CFG_LAYER_WPOS;
		peak = fitw->GetParameter(1);
		sigma = fitw->GetParameter(2);
		error = fitw->GetParError(1);
		treeTS.Fill();
		fits.setFit(w, positive, peak, error, sigma);

		c2.cd(4);
		timesums.layer_uneg->Draw();
		timesums.layer_uneg->Fit("gaus");
		TF1 *fitun = timesums.layer_uneg->GetFunction("gaus");
		layer = CFG_LAYER_UNEG;
		peak = fitun->GetParameter(1);
		sigma = fitun->GetParameter(2);
		error = fitun->GetParError(1);
		treeTS.Fill();
		fits.setFit(u, negative, peak, error, sigma);

		c2.cd(5);
		timesums.layer_vneg->Draw();
		timesums.layer_vneg->Fit("gaus");
		TF1 *fitvn = timesums.layer_vneg->GetFunction("gaus");
		layer = CFG_LAYER_VNEG;
		peak = fitvn->GetParameter(1);
		sigma = fitvn->GetParameter(2);
		error = fitvn->GetParError(1);
		treeTS.Fill();
		fits.setFit(v, negative, peak, error, sigma);

		c2.cd(6);
		timesums.layer_wneg->Draw();
		timesums.layer_wneg->Fit("gaus");
		TF1 *fitwn = timesums.layer_wneg->GetFunction("gaus");
		layer = CFG_LAYER_WNEG;
		peak = fitwn->GetParameter(1);
		sigma = fitwn->GetParameter(2);
		error = fitwn->GetParError(1);
		treeTS.Fill();
		fits.setFit(w, negative, peak, error, sigma);
	}

	else if (userDet == pos) {
		//set up canvas for time sums - 3 for each detector - therefore only 3 in one det in use
		//TPad::Divide() specifies number of vertical and horizontal slices of canvas
		c2.Divide(1, 3);

		c2.cd(1);
		timesums.layer_upos->Draw();
		timesums.layer_upos->Fit("gaus");
		gStyle->SetOptFit(0011);
		TF1 *fitu = timesums.layer_upos->GetFunction("gaus");
		layer = CFG_LAYER_UPOS;
		peak = fitu->GetParameter(1);
		sigma = fitu->GetParameter(2);
		error = fitu->GetParError(1);
		treeTS.Fill();
		fits.setFit(u, positive, peak, error, sigma);
		//Want to display fit parameters on timesums plots
		//timesums.layer_upos->SetOptFit();

		c2.cd(2);
		timesums.layer_vpos->Draw();
		timesums.layer_vpos->Fit("gaus");
		TF1 *fitv = timesums.layer_vpos->GetFunction("gaus");
		layer = CFG_LAYER_VPOS;
		peak = fitv->GetParameter(1);
		sigma = fitv->GetParameter(2);
		error = fitv->GetParError(1);
		treeTS.Fill();
		fits.setFit(v, positive, peak, error, sigma);

		c2.cd(3);
		timesums.layer_wpos->Draw();
		timesums.layer_wpos->Fit("gaus");
		TF1 *fitw = timesums.layer_wpos->GetFunction("gaus");
		layer = CFG_LAYER_WPOS;
		peak = fitw->GetParameter(1);
		sigma = fitw->GetParameter(2);
		error = fitw->GetParError(1);
		treeTS.Fill();
		fits.setFit(w, positive, peak, error, sigma);
	}

	else {
		//detmust be negative
		//set up canvas for time sums - 3 for each detector - therefore only 3 in one det in use
		//TPad::Divide() specifies number of vertical and horizontal slices of canvas
		c2.Divide(1, 3);
		c2.cd(1);
		timesums.layer_uneg->Draw();
		timesums.layer_uneg->Fit("gaus");
		TF1 *fitun = timesums.layer_uneg->GetFunction("gaus");
		layer = CFG_LAYER_UNEG;
		peak = fitun->GetParameter(1);
		sigma = fitun->GetParameter(2);
		error = fitun->GetParError(1);
		treeTS.Fill();
		fits.setFit(u, negative, peak, error, sigma);

		c2.cd(2);
		timesums.layer_vneg->Draw();
		timesums.layer_vneg->Fit("gaus");
		TF1 *fitvn = timesums.layer_vneg->GetFunction("gaus");
		layer = CFG_LAYER_VNEG;
		peak = fitvn->GetParameter(1);
		sigma = fitvn->GetParameter(2);
		error = fitvn->GetParError(1);
		treeTS.Fill();
		fits.setFit(v, negative, peak, error, sigma);

		c2.cd(3);
		timesums.layer_wneg->Draw();
		timesums.layer_wneg->Fit("gaus");
		TF1 *fitwn = timesums.layer_wneg->GetFunction("gaus");
		layer = CFG_LAYER_WNEG;
		peak = fitwn->GetParameter(1);
		sigma = fitwn->GetParameter(2);
		error = fitwn->GetParError(1);
		treeTS.Fill();
		fits.setFit(w, negative, peak, error, sigma);
	}
	//Want to write timesum information to tree for accessing later in program, also to save to csv such that
	//ts info for all runs can be accessed at later dates without rerunning code
	treeTS.Write();

	//Checks timesums are within 2sigma of fitted peak
	checkTimeSums(data, fits, userDet);

	//takes timedifferences of MCP hits for each event within a group
	//identifies particle and stores in MCPTDiff double
	//stored is the particleID and timedifference relative to positron
	//identifyAndTime(data);

	// want to index number of particles and bool triple for each each group
	//selectPosIonElec(data);

	//check each particle hit has enough information to reconstruct X Y position
	//not needed for ion
	//does not need userDet check
	checkReconstructable(data);

	//now need to check for all three particles (ion, pos, elec) and that the pos and elec have a reconBool

	//calculate pitch propogation
	PitchPropSet Pitches = calculatePitchProp(fits);

	//Copy over triple coincidences with reconstrutable particle hits to new dataset

	//does not need userDet
	DataSet *reconData = sortReconData(data);
	/** 
	 * Convert time to position info
	 *First off need to get U,V,W from u1 u2, v1 v2, w1 w2
	 *
	 */
	convertLayerPosition(reconData, Pitches, userDet);

	convertCartesianPosition(reconData, userDet);
	
	gStyle->SetPalette(1);
	double_t w = 800;
	double_t h = 800;

	//histogram detector images with 2D histogram
	//can have userDet implementation, not currently implemented
	HistogramXY XYpositions = histogramXYPositions(reconData);

	//draw the detector images
	TCanvas c3("c3", "Third Canvas", w, h);
	//works better with more runs and a contour plot, 
	//since plotting individual runs intensity is 1, need to implement contours
	//XYpositions.positronDET->Draw("colz");
	if (userDet == bothDet) {
		XYpositions.positronDET->Draw("colz");
		TCanvas c4("c4", "Fourth Canvas", w, h);
		XYpositions.electronDET->Draw("colz");
	}
	else if (userDet == pos) {
		XYpositions.positronDET->Draw("colz");
	}
	else {
		XYpositions.electronDET->Draw("colz");
	}

	//Calibrate Mask Section


	
	//TCanvas c5("c5", "Fifth Canvas", w, h);
	//XYpositions.ionDET->Draw("cont0");

	//if you want individual layers use this program, not currently implemented with userDet

	//if (userDet == negDet) {
		HistogramElecLayers UVWlayers = histogramElectronLayers(reconData);

		//Delete comment /**\ if you want to calculate layers UVW and graph  
				
		TCanvas c6("c6", "Sixth Canvas", w, h);
		//UVWlayers.UVlayers->SetMarkerColor(kBlue);
		//UVWlayers.UVlayers->SetLineColor(kBlue);
		//UVWlayers.UVlayers->Draw("hist");
		UVWlayers.UWlayers->SetMarkerColor(kRed);
		UVWlayers.UWlayers->SetLineColor(kRed);
		UVWlayers.UWlayers->Draw("hist");
		//UVWlayers.VWlayers->SetLineColor(kBlack);
		//UVWlayers.VWlayers->Draw("SameHist");
		c6.SetTitle("UVW Layers Combined; x (mm); y (mm)");
		TLegend* leg = new TLegend(0.1,0.7,0.3,0.9, "Layers");
		leg->Draw();
		//leg->AddEntry(UVWlayers.UVlayers, "UV layer");
		leg->AddEntry(UVWlayers.UWlayers, "UW layer");
		//leg->AddEntry(UVWlayers.VWlayers, "WV layer");
		cout << "hist layers run" << endl;
		c6.Update();
		//}

		calibrateLayersHist UVWMasklayers = histogramMaskLayers(reconData);

		TCanvas c7("c7", "Seventh Canvas");
		//UVWMasklayers.UVMasklayer->SetLineColor(kBlue);
		//UVWMasklayers.UVMasklayer->Draw("hist");
		UVWMasklayers.UWMasklayer->SetLineColor(kRed);
		UVWMasklayers.UWMasklayer->Draw("hist");
		//UVWMasklayers.VWMasklayer->SetLineColor(kBlack);
		//UVWMasklayers.VWMasklayer->Draw("SameHist");
		//TLegend* leg = new TLegend("Layers");
		//leg->Draw();
		//leg->AddEntry(UVWMasklayers.UVMasklayer, "UV layer");
		//leg->AddEntry(UVWMasklayers.UWMasklayer, "UW layer");
		//leg->AddEntry(UVWMasklayers.VWMasklayer, "WV layer");
		cout << "hist layers run" << endl;
		c7.Update();

	//differenceOfLayers(reconData);

	/*
	HistogramDIFF UVWdifferences = histogramElecLayerDiff(reconData);
	TCanvas c7("c7", "Layer Differences");
	c7.Divide(2, 3);
	c7.cd(1);
	UVWdifferences.ydiffuv_uw->Draw();
	c7.cd(2);
	UVWdifferences.ydiffuv_vw->Draw();
	c7.cd(3);
	UVWdifferences.xdiffuv_vw->Draw();
	c7.cd(4);
	UVWdifferences.ydiffuw_vw->Draw();
	c7.cd(5);
	UVWdifferences.xdiffuw_vw->Draw();
	*/

	rootapp->Run();
	
	//should print out metafile of setup, AND csv dump of data. 

    return 0;
}

