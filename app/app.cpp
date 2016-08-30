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
#include "dirent.h"
#include <stdio.h>
#include <string.h>
#include <TAxis.h>
#include "HistUVWPositions.h"
#include "histlayerSums.h"
#include "TH1.h"

using namespace std;

/*CALIBRATION PROGRAM FOR DETECTORS
* Note: data being parsed into program must be detectors with the calibration mask on
* Two options for loading in data: 1. read, read in the raw data trees, process timesums for the first tree, then obtain X,Y images. 
* After initial processing, this option also loads all layer timing info, u1, u2, v1..... etc into a tree that can be used for the second option.
* 2. load, This option loads the tree produced in option 1, this allows for initial processing and loading to be skipped. Essentially data is loaded in
* at the point where it is ready to be orocessed using the variables in need of calibration. Allows for quicker chanigng of variables and processing. 
* This program does not select out for particles and will use all reconstructable hits to produce an image
* Author: Tamara J Babij 
* version control of software found on: https://github.com/TamaraJBabij/CalibrateDetectors/  */

int main(int argc, char* argv[]) {
	//Initial function to set up the debug environment
	int setUpDebugEnvironment();
	//initialises root app
	TApplication* rootapp = new TApplication("C:/Users/TamaraB/Documents/GitHub/CalibrateDetectors", &argc, argv);

	/*
	This program can be run for lower intensity beams, however maxes out memory above 60 trees
	DataSet* data = scanFiles();
	*/

	/*Implementation of detinput allows for calibration of one detector at a time*/
	string detinput;
	cout << "what detectors do you want to image: neg, pos or both? " << endl;
	cin >> detinput;
	imagingDetectors userDet;
	if (detinput.compare("pos") == 0) {
		userDet = posDet;
		//cout << "pos" << endl;
	}
	else if (detinput.compare("neg") == 0) {
		userDet = negDet;
		//cout << "neg" << endl;
	}
	else if (detinput.compare("both") == 0) {
		userDet = bothDet;
		//cout << "both" << endl;
	}
	else {
		cout << "Input does not match a valid detector" << endl;
		exit(1);
	}

	string operationInfo;
	cout << "Timesums (type: time), Image (type: imag) or both (type: both)? " << endl;
	cin >> operationInfo;
	commandInfo userInfo;

	if (operationInfo.compare("imag") == 0) {
		userInfo = imageInfo;
	}
	else if (operationInfo.compare("time") == 0) {
		userInfo = timesumInfo;
	}
	else if (operationInfo.compare("both") == 0) {
		userInfo = bothInfo;
	}
	else {
		cout << "Input does not match a valid command" << endl;
		exit(1);
	}

	HistogramXY XYpositions;

	//scans folder created by the DAQ software and loads all tree ( .root) files into the dataset
	//not currently neccasary for the load options
	string fileLocation;
	cout << "what is the directory? remember to add a backslash at the end" << endl;
	cin >> fileLocation;

	//User can select to only image (reconstrcut X,Y) for a certain timing window
	/*
	int minImageTime = 0;
	int maxImageTime = 0;
	string imagingTOFinput;
	cout << "Would you like to only image a certain subset of TOF? (y/n)" << endl;
	cin >> imagingTOFinput;
	imagingTOFInfo 
	if ();
	*/

	int filenumber = 0;
	DIR* dir;
	dirent* pdir;
	dir = opendir(fileLocation.c_str());
	HistogramTimeSums timesums;
	bool firstFile = true;
	//delcare here as whatever curly brackets the variable is declared in it stays in (called scope)
	FitSet fits;
	gStyle->SetPalette(1);
	double_t canvasWidth = 800;
	double_t h = 800;
	PitchPropSet Pitches;
	//calculate pitch propogation
	Pitches = calculatePitchProp();
	TCanvas timesumsCanvas("Time sums canvas", "Time Sums");
	TCanvas XYPosDet("Positive Detector", "XY Positions Positron Detector", canvasWidth, h);
	TCanvas XYNegDet("Negative Detector", "XY Positions", canvasWidth, h);
	TCanvas *TimeSpectra = new TCanvas("Time Spectrum", "Time Spectrum (abs)");

	
	HistogramPair histTimeSpec;
	TCanvas UVWNeglayersCanvas("UVW layers Canvas", "UVW Negative Detector", canvasWidth, h);
	HistogramElecLayers UVWlayers;

	if (userDet == bothDet) {
		XYpositions.positronDET = new TH2D("positronDET", "Positrons", 250, -60, 60, 250, -60, 60);
		XYpositions.electronDET = new TH2D("electronDET", "Electrons", 250, -60, 60, 250, -60, 60);
		XYPosDet.cd();
		XYpositions.positronDET->Draw("colz");
		XYNegDet.cd();
		XYpositions.electronDET->Draw("colz");
		histTimeSpec.positive = new TH1D("hpos", "TimeSpectrum positive", 500, 0, 32000);
		histTimeSpec.negative = new TH1D("hneg", "TimeSpectrum negative", 500, 0, 32000);
		TimeSpectra->Divide(1, 2);
		TimeSpectra->cd(1);
		TimeSpectra->cd(1)->SetLogy();
		histTimeSpec.positive->Draw();
		TimeSpectra->cd(2);
		histTimeSpec.negative->Draw();
	}
	else if (userDet == posDet) {
		XYpositions.positronDET = new TH2D("positronDET", "Positrons", 400, -60, 60, 400, -60, 60);
		XYPosDet.cd();
		XYpositions.positronDET->Draw("colz");
		histTimeSpec.positive = new TH1D("hpos", "TimeSpectrum positive", 500, -2000, 32000);
		TimeSpectra->cd();
		histTimeSpec.positive->Draw();
	}
	else if (userDet == negDet) {
		XYpositions.electronDET = new TH2D("electronDET", "Electrons", 400, -60, 60, 400, -60, 60);
		XYNegDet.cd();
		XYpositions.electronDET->Draw("colz");
		histTimeSpec.negative = new TH1D("hpos", "TimeSpectrum negative", 500, -2000, 32000);
		TimeSpectra->cd();
		histTimeSpec.negative->Draw();
		UVWNeglayersCanvas.cd();
		UVWlayers.UVlayers = new TH2D("electronDET", "UV layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.UWlayers = new TH2D("electronDET", "UW layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.VWlayers = new TH2D("electronDET", "VW layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.UVlayers->SetMarkerColor(kBlue);
		UVWlayers.UVlayers->SetLineColor(kBlue);
		UVWlayers.UWlayers->SetMarkerColor(kRed);
		UVWlayers.UWlayers->SetLineColor(kRed);
		UVWlayers.VWlayers->SetLineColor(kBlack);
		UVWlayers.UVlayers->Draw("hist");
		UVWlayers.UWlayers->Draw("SameHist");
		UVWlayers.VWlayers->Draw("SameHist");
		UVWNeglayersCanvas.SetTitle("UVW Layers Combined; x (mm); y (mm)");
		TLegend* elecLegend = new TLegend(0.1, 0.7, 0.3, 0.9, "Layers");
		elecLegend->AddEntry(UVWlayers.UVlayers, "UV layer");
		elecLegend->AddEntry(UVWlayers.UWlayers, "UW layer");
		elecLegend->AddEntry(UVWlayers.VWlayers, "WV layer");
		elecLegend->Draw();
		
	}
	//Histograms the positron and electron layers, need to change name
	
	// scans through all files in the folder
	//  parses in all .root files (trees) created by DAQ software
	vector<char*> files;
					
		if (dir != NULL) {
			while (pdir = readdir(dir)) {
				char* fileName = pdir->d_name;
				if (strlen(fileName) > 5 && !strcmp(fileName + strlen(fileName) - 5, ".root")) {

					DataSet* data = new DataSet();
					//using string Folder Name acquire intiial tree
					//Initial tree is given by ReMinumber with no underscore number
					//cout << fileName << endl;
					char str[256];
					strcpy(str, fileLocation.c_str());
					strcat(str, fileName);
					//cout << str << endl;
					cout << filenumber << endl;
					filenumber++;

					TFile* rawFile = TFile::Open(str);
					TTree* rawTree = (TTree*)rawFile->Get("T");
					loadFromTreeDataSet(rawTree, data);
					rawFile->Close();
					//Associate hits into events, where event is a single particle/ion hit on the detector. Events are sorted by group
					constructEvents(data);

					plotTimeSpectraDS(data, userDet, &histTimeSpec);

					// construct timesum histograms on first file
					if (firstFile) {
						if (userInfo == timesumInfo || userInfo == bothInfo) {
							timesums = calculateTimeSums(data, userDet);
							firstFile = false;
							//Store fit parameters into a tree for later accessing
							// setting up tree
							TFile file("TimeSumTree.root", "recreate");
							TTree treeTS("TimeSumPeaks", "simple tree that stores timesum peaks and sigma");

							Double_t peak, sigma, error;
							//Will define layer numbers in configuration.h file
							//not sure if text/string can be stored in trees
							Double_t layer;
							treeTS.Branch("Layer", &layer);
							treeTS.Branch("Peak", &peak);
							treeTS.Branch("Sigma", &sigma);
							treeTS.Branch("Error", &error);

							/* CALCULATES AND FITS TIMESUMS BASED ON DETECTORS */
							if (userDet == bothDet) {

								//set up canvas for time sums - 3 for each detector - 6 in total
								//TPad::Divide() specifies number of vertical and horizontal slices of canvas
								timesumsCanvas.Divide(2, 3);

								timesumsCanvas.cd(1);
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

								timesumsCanvas.cd(2);
								timesums.layer_vpos->Draw();
								timesums.layer_vpos->Fit("gaus");
								TF1 *fitv = timesums.layer_vpos->GetFunction("gaus");
								layer = CFG_LAYER_VPOS;
								peak = fitv->GetParameter(1);
								sigma = fitv->GetParameter(2);
								error = fitv->GetParError(1);
								treeTS.Fill();
								fits.setFit(v, positive, peak, error, sigma);

								timesumsCanvas.cd(3);
								timesums.layer_wpos->Draw();
								timesums.layer_wpos->Fit("gaus");
								TF1 *fitw = timesums.layer_wpos->GetFunction("gaus");
								layer = CFG_LAYER_WPOS;
								peak = fitw->GetParameter(1);
								sigma = fitw->GetParameter(2);
								error = fitw->GetParError(1);
								treeTS.Fill();
								fits.setFit(w, positive, peak, error, sigma);

								timesumsCanvas.cd(4);
								timesums.layer_uneg->Draw();
								timesums.layer_uneg->Fit("gaus");
								TF1 *fitun = timesums.layer_uneg->GetFunction("gaus");
								layer = CFG_LAYER_UNEG;
								peak = fitun->GetParameter(1);
								sigma = fitun->GetParameter(2);
								error = fitun->GetParError(1);
								treeTS.Fill();
								fits.setFit(u, negative, peak, error, sigma);

								timesumsCanvas.cd(5);
								timesums.layer_vneg->Draw();
								timesums.layer_vneg->Fit("gaus");
								TF1 *fitvn = timesums.layer_vneg->GetFunction("gaus");
								layer = CFG_LAYER_VNEG;
								peak = fitvn->GetParameter(1);
								sigma = fitvn->GetParameter(2);
								error = fitvn->GetParError(1);
								treeTS.Fill();
								fits.setFit(v, negative, peak, error, sigma);

								timesumsCanvas.cd(6);
								timesums.layer_wneg->Draw();
								timesums.layer_wneg->Fit("gaus");
								TF1 *fitwn = timesums.layer_wneg->GetFunction("gaus");
								layer = CFG_LAYER_WNEG;
								peak = fitwn->GetParameter(1);
								sigma = fitwn->GetParameter(2);
								error = fitwn->GetParError(1);
								treeTS.Fill();
								fits.setFit(w, negative, peak, error, sigma);
								timesumsCanvas.Modified();
								timesumsCanvas.Update();
							}

							else if (userDet == posDet) {
								//set up canvas for time sums - 3 for each detector - therefore only 3 in one det in use
								//TPad::Divide() specifies number of vertical and horizontal slices of canvas
								timesumsCanvas.Divide(1, 3);

								timesumsCanvas.cd(1);
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

								timesumsCanvas.cd(2);
								timesums.layer_vpos->Draw();
								timesums.layer_vpos->Fit("gaus");
								TF1 *fitv = timesums.layer_vpos->GetFunction("gaus");
								layer = CFG_LAYER_VPOS;
								peak = fitv->GetParameter(1);
								sigma = fitv->GetParameter(2);
								error = fitv->GetParError(1);
								treeTS.Fill();
								fits.setFit(v, positive, peak, error, sigma);

								timesumsCanvas.cd(3);
								timesums.layer_wpos->Draw();
								timesums.layer_wpos->Fit("gaus");
								TF1 *fitw = timesums.layer_wpos->GetFunction("gaus");
								layer = CFG_LAYER_WPOS;
								peak = fitw->GetParameter(1);
								sigma = fitw->GetParameter(2);
								error = fitw->GetParError(1);
								treeTS.Fill();
								fits.setFit(w, positive, peak, error, sigma);
								timesumsCanvas.Modified();
								timesumsCanvas.Update();
							}

							else if (userDet == negDet) {
								//detmust be negative
								//set up canvas for time sums - 3 for each detector - therefore only 3 in one det in use
								//TPad::Divide() specifies number of vertical and horizontal slices of canvas
								timesumsCanvas.Divide(1, 3);
								timesumsCanvas.cd(1);
								timesums.layer_uneg->Draw();
								timesums.layer_uneg->Fit("gaus");
								TF1 *fitun = timesums.layer_uneg->GetFunction("gaus");
								layer = CFG_LAYER_UNEG;
								peak = fitun->GetParameter(1);
								sigma = fitun->GetParameter(2);
								error = fitun->GetParError(1);
								treeTS.Fill();
								fits.setFit(u, negative, peak, error, sigma);

								timesumsCanvas.cd(2);
								timesums.layer_vneg->Draw();
								timesums.layer_vneg->Fit("gaus");
								TF1 *fitvn = timesums.layer_vneg->GetFunction("gaus");
								layer = CFG_LAYER_VNEG;
								peak = fitvn->GetParameter(1);
								sigma = fitvn->GetParameter(2);
								error = fitvn->GetParError(1);
								treeTS.Fill();
								fits.setFit(v, negative, peak, error, sigma);

								timesumsCanvas.cd(3);
								timesums.layer_wneg->Draw();
								timesums.layer_wneg->Fit("gaus");
								TF1 *fitwn = timesums.layer_wneg->GetFunction("gaus");
								layer = CFG_LAYER_WNEG;
								peak = fitwn->GetParameter(1);
								sigma = fitwn->GetParameter(2);
								error = fitwn->GetParError(1);
								treeTS.Fill();
								fits.setFit(w, negative, peak, error, sigma);
								timesumsCanvas.Modified();
								timesumsCanvas.Update();
							}

							//Want to write timesum information to tree for accessing later in program, also to save to csv such that
							//ts info for all runs can be accessed at later dates without rerunning code
							treeTS.Write();
						}
						
					}

					if (userInfo == timesumInfo)break;

					if (userInfo == imageInfo || userInfo == bothInfo) {
						//Provides time spectrum for positive/negative detectors


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

						//does not need userDet
						DataSet *reconData = sortReconData(data);

						convertLayerPosition(reconData, Pitches, userDet);

						convertCartesianPosition(reconData, userDet, &XYpositions, &UVWlayers);

						XYPosDet.Modified();
						XYPosDet.Update();
						XYNegDet.Modified();
						XYNegDet.Update();
						UVWNeglayersCanvas.Modified();
						UVWNeglayersCanvas.Update();
						//TimeSpectra->Modified();
						//TimeSpectra->Update();
						

						//histogram detector images with 2D histogram
						//can have userDet implementation, not currently implemented
						//histogramXYPositions(reconData, XYpositions);

						//draw the detector images
						//histogramElectronLayers(reconData, &UVWlayers, userDet);

						//histogramMaskLayers(reconData, &UVWMasklayers);
					}

					//needed to make root canvases interactive 
					//Lives updates the graphs
					
					
					rootapp->Draw();

					delete data;
				}
			}
		}
	
	//else {
		//cout << "Directory does not exist, exiting" << endl;
		//exit(1);
	//}

	closedir(dir);
	TimeSpectra->Modified();
	TimeSpectra->Update();
	rootapp->Draw();

	rootapp->Run();

	//should print out metafile of setup, AND csv dump of data. 

	return 0;
		
}

