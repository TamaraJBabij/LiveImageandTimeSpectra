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

int main(int argc, char* argv[]) {
	//Initial function to set up the debug environment
	int setUpDebugEnvironment();
	//initialises root app
	TApplication* rootapp = new TApplication("example", &argc, argv);
	TFile* rawFile = TFile::Open("../01102015.root");
	TTree* rawTree = (TTree*)rawFile->Get("T");
	DataSet* data = loadFromTree(rawTree);
	rawFile->Close();
	//TTree* tree = readWriteTree(rawTree);

	TFile* raw2File = TFile::Open("../02102015.root");
	TTree* raw2Tree = (TTree*)raw2File->Get("T");
	loadFromTreeDataSet(raw2Tree, data);
	raw2File->Close();

	TFile* raw3File = TFile::Open("../03102015.root");
	TTree* raw3Tree = (TTree*)raw3File->Get("T");
	loadFromTreeDataSet(raw3Tree, data);
	raw3File->Close();

	TFile* raw4File = TFile::Open("../06102015.root");
	TTree* raw4Tree = (TTree*)raw4File->Get("T");
	loadFromTreeDataSet(raw4Tree, data);
	raw4File->Close();

	TFile* raw5File = TFile::Open("../07102015.root");
	TTree* raw5Tree = (TTree*)raw5File->Get("T");
	loadFromTreeDataSet(raw5Tree, data);
	raw5File->Close();

	TFile* raw6File = TFile::Open("../08102015.root");
	TTree* raw6Tree = (TTree*)raw6File->Get("T");
	loadFromTreeDataSet(raw6Tree, data);
	raw6File->Close();

	TFile* raw7File = TFile::Open("../09102015.root");
	TTree* raw7Tree = (TTree*)raw7File->Get("T");
	loadFromTreeDataSet(raw7Tree, data);
	//DataSet* data = loadFromTree(raw7Tree);
	raw7File->Close();

	//tree->Draw("Time");
	HistogramPair hists = plotTimeSpectraDS(data);

	// need to switch between canvases in root in order to creat two histograms
	//any instance of TCanvas can be subdivided into independent graphical areas, 
	//called pads (by default, a canvas contain a single pad, occupying the whole space - TCanvas inherits from TPad).


	TCanvas c1("c1", "First Canvas");
	//TPad::Divide() specifies number of vertical and horizontal slices of canvas
	c1.Divide(1,2);
	c1.cd(1);
	hists.positive->Draw();
	//hists.positive->Write();
	//Just defual draw hist option offers irrelavnt info for time spectrum
	//gStyle->SetOptStat("");
	c1.cd(2);
	//TFile f("histos.root", "new");
	hists.negative->Draw();
	//hists.negative->Write();

	//Associate hits into events, where event is a single particle/ion hit on the detector. Events are sorted by group
	constructEvents(data);

	// construct timesum histograms
	HistogramTimeSums timesums = calculateTimeSums(data);

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
	//set up canvas for time sums - 3 for each detector - 6 in total
	//TPad::Divide() specifies number of vertical and horizontal slices of canvas
	c2.Divide(2, 3);
	FitSet fits;

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
	fits.setFit(u,positive,peak,error,sigma);
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

	//Want to write timesum information to tree for accessing later in program, also to save to csv such that
	//ts info for all runs can be accessed at later dates without rerunning code
	treeTS.Write();

	//Checks timesums are within 2sigma of fitted peak
	checkTimeSums(data, fits);

	//takes timedifferences of MCP hits for each event within a group
	//identifies particle and stores in MCPTDiff double
	//stored is the particleID and timedifference relative to positron
	identifyAndTime(data);

	// want to index number of particles and bool triple for each each group
	selectPosIonElec(data);

	//check each particle hit has enough information to reconstruct X Y position
	//not needed for ion
	checkReconstructable(data);

	//now need to check for all three particles (ion, pos, elec) and that the pos and elec have a reconBool

	//calculate pitch propogation
	PitchPropSet Pitches = calculatePitchProp(fits);

	//Copy over triple coincidences with reconstrutable particle hits to new dataset


	DataSet *reconData = sortReconData(data);
	/** 
	 * Convert time to position info
	 *First off need to get U,V,W from u1 u2, v1 v2, w1 w2
	 *
	 */
	convertLayerPosition(reconData, Pitches);

	convertCartesianPosition(reconData);
	
	gStyle->SetPalette(1);
	double_t w = 800;
	double_t h = 800;

	//histogram detector images with 2D histogram
	HistogramXY XYpositions = histogramXYPositions(reconData);
	//draw the detector images
	TCanvas c3("c3", "Third Canvas", w, h);
	//works better with more runs and a contour plot, 
	//since plotting individual runs intensity is 1, need to implement contours
	//XYpositions.positronDET->Draw("colz");
	XYpositions.positronDET->Draw("cont0");

	TCanvas c4("c4", "Fourth Canvas", w, h);
	XYpositions.electronDET->Draw("cont0");

	TCanvas c5("c5", "Fifth Canvas", w, h);
	XYpositions.ionDET->Draw("cont0");

	HistogramElecLayers UVWlayers = histogramElectronLayers(reconData);

	TCanvas c6("c6", "Sixth Canvas", w, h);
	UVWlayers.UVlayers->SetMarkerColor(kBlue);
	UVWlayers.UVlayers->SetLineColor(kBlue);
	UVWlayers.UVlayers->Draw("hist");
	UVWlayers.UWlayers->SetMarkerColor(kRed);
	UVWlayers.UWlayers->SetLineColor(kRed);
	UVWlayers.UWlayers->Draw("SameHist");
	UVWlayers.VWlayers->SetLineColor(kBlack);
	UVWlayers.VWlayers->Draw("SameHist");
	c6.SetTitle("UVW Layers Combined; x (mm); y (mm)");
	TLegend* leg = new TLegend(0.1,0.7,0.3,0.9, "Layers");
	leg->Draw();
	leg->AddEntry(UVWlayers.UVlayers, "UV layer");
	leg->AddEntry(UVWlayers.UWlayers, "UW layer");
	leg->AddEntry(UVWlayers.VWlayers, "WV layer");
	c6.Update();

	differenceOfLayers(reconData);

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

	rootapp->Run();
	
	//should print out metafile of setup, AND csv dump of data. 

    return 0;
}

