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
#include "math.h"
using namespace std;

/*Detector XY Image, Timesums and Time of Flight spectra
* Two options for processing data: 1. imag, read in the raw data trees, process timesums for the first tree, then obtain X,Y images. 
* 2. time, produces timesums for the first tree present in selected folder 
* This program does not select out for particles and will use all reconstructable hits to produce an image
* Author: Tamara J Babij 
* version control of software found on: https://github.com/TamaraJBabij/LiveImageandTimeSpectra/ */

int main(int argc, char* argv[]) {
	//Initial function to set up the debug environment
	int setUpDebugEnvironment();
	//initialises root app
	TApplication* rootapp = new TApplication("C:/Users/TamaraB/Documents/GitHub/CalibrateDetectors", &argc, argv);

	/*Implementation of detinput allows for imaging either 1 detector or both detectors*/
	string detinput;
	cout << "what detectors do you want to image: neg, pos or both? " << endl;
	cin >> detinput;
	imagingDetectors userDet;
	if (detinput.compare("pos") == 0) {
		//Images only the positive (positron) detector
		userDet = posDet;
	}
	else if (detinput.compare("neg") == 0) {
		//Images only the negative (electron) detector
		userDet = negDet;
	}
	else if (detinput.compare("both") == 0) {
		//Images both the positive and negative detectors
		userDet = bothDet;
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
		//Produces only the detector images for selected detectors as defined by userDet
		userInfo = imageInfo;
	}
	else if (operationInfo.compare("time") == 0) {
		//Produces only timesums for first tree in the selected folder, for selected detectors as defined by userDet
		userInfo = timesumInfo;
	}
	else if (operationInfo.compare("both") == 0) {
		//Produces both timesums and detector images for selected detectors as defined by userDet
		userInfo = bothInfo;
	}
	else {
		cout << "Input does not match a valid command" << endl;
		exit(1);
	}

	//Data container for the histograms for the X,Y positions for the positive and negative particle reconstructions
	HistogramXY XYpositions;

	//scans folder created by the DAQ software and loads all tree ( .root) files into the dataset
	//not currently neccasary for the load options
	string fileLocation;
	cout << "what is the directory? remember to add a backslash at the end" << endl;
	cin >> fileLocation;

	//User can select to only image (reconstrcut X,Y) for a certain timing window
	// As set by user input max and min imagaing times (ns)
	double minImageTime = 0.0;
	double maxImageTime = 0.0;
	string imagingTOFinput;
	cout << "Would you like to only image a certain subset of TOF? (y/n)" << endl;
	cin >> imagingTOFinput;
	imagingTOFInfo imageUserInfo;
		if (imagingTOFinput.compare("n") == 0) {
			imageUserInfo = imageallTOF;
			
		}
		else if (imagingTOFinput.compare("y") == 0) {
			imageUserInfo = imagesubsetTOF;
			//if yes for image subset, then ask user for subset timing range
			cout << "what min image time(ns)?" << endl;
			cin >> minImageTime;
			cout << "what mac image time(ns)?" << endl;
			cin >> maxImageTime;
			//check that timing min << timing max
			if (minImageTime > maxImageTime) {
				cout << "Not a valid range" << endl;
				exit(1);
			}
		}
	


	int filenumber = 0;
	DIR* dir;
	dirent* pdir;
	dir = opendir(fileLocation.c_str());
	HistogramTimeSums timesums;
	bool firstFile = true;
	//delcare here as whatever curly brackets the variable is declared in it stays in (called scope)
	//FitSet contains the the values of a Gaus being fit to the timesums histograms
	//This allows for a check of valid timesums within some nsigma of the the gaussian peak
	FitSet fits;
	//Set Canvas hieght and width such that detector X,Y images canvas' are square
	gStyle->SetPalette(1);
	double_t canvasWidth = 800;
	double_t h = 800;
	//Data container for all pitch propagation values and layer offsets
	PitchPropSet Pitches;
	//calculate pitch propogation
	Pitches = calculatePitchProp();
	//Set up canvas to contain appropriate histograms
	TCanvas timesumsCanvas("Time sums canvas", "Time Sums");
	TCanvas XYPosDet("Positive Detector", "XY Positions Positron Detector", canvasWidth, h);
	TCanvas XYNegDet("Negative Detector", "XY Positions", canvasWidth, h);
	TCanvas *TimeSpectra = new TCanvas("Time Spectrum", "Time Spectrum (abs)");
	//TCanvas UVPosDet(" UV Positive Detector", "UV Positions Positron Detector", canvasWidth, h);
	//TCanvas UWPosDet("UW Positive Detector", "UW Positions Positron Detector", canvasWidth, h);
	//TCanvas VWPosDet("VW Positive Detector", "VW Positions Positron Detector", canvasWidth, h);
	TCanvas UVWPoslayersCanvas("UVW Positive Detector", "UVW Positions Positron Detector", canvasWidth, h);
	// Histogram container for the time spectrum produced for the negative and positive detectors
	HistogramPair histTimeSpec;
	TCanvas UVWNeglayersCanvas("UVW layers Canvas", "UVW Negative Detector", canvasWidth, h);
	//Histogram container for the histograms of each of the layer X,Y reconstructions 
	//Histograms the positron and electron layers, need to change name
	HistogramElecLayers UVWlayers;
	TCanvas UVPoslayersCanvas("UV layers Canvas", "UV Negative Detector", canvasWidth, h);
	TCanvas UWPoslayersCanvas("UW layers Canvas", "UW Negative Detector", canvasWidth, h);
	TCanvas VWPoslayersCanvas("VW layers Canvas", "VW Negative Detector", canvasWidth, h);


	//Draw canvases based on userDet option
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
		TimeSpectra->cd(2)->SetLogy();
		histTimeSpec.negative->Draw();
		UVWlayers.UVPoslayers = new TH2D("Positive Detector", "UV layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.UWPoslayers = new TH2D("Positive Detector", "UW layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.VWPoslayers = new TH2D("Positive Detector", "VW layer", 400, -60, 60, 400, -60, 60);
		UVWlayers.UVPoslayers->SetMarkerColor(kBlue);
		UVWlayers.UVPoslayers->SetLineColor(kBlue);
		UVWlayers.UWPoslayers->SetMarkerColor(kRed);
		UVWlayers.UWPoslayers->SetLineColor(kRed);
		UVWlayers.VWPoslayers->SetLineColor(kBlack);
		UVWPoslayersCanvas.cd();
		UVWlayers.UVPoslayers->Draw("hist");
		UVWlayers.UWPoslayers->Draw("SameHist");
		UVWlayers.VWPoslayers->Draw("SameHist");
		UVWPoslayersCanvas.SetTitle("UVW Layers Combined; x (mm); y (mm)");
		TLegend* PosLegend = new TLegend(0.1, 0.7, 0.3, 0.9, "Layers");
		PosLegend->AddEntry(UVWlayers.UVPoslayers, "UV layer");
		PosLegend->AddEntry(UVWlayers.UWPoslayers, "UW layer");
		PosLegend->AddEntry(UVWlayers.VWPoslayers, "WV layer");
		PosLegend->Draw();
		UVPoslayersCanvas.cd();
		UVWlayers.UVPoslayers->Draw();
		UWPoslayersCanvas.cd();
		UVWlayers.UWPoslayers->Draw(); 
		VWPoslayersCanvas.cd();
		UVWlayers.VWPoslayers->Draw();
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
	
	//Want to crate a profile of the beam spot
	TH1D* xinty = new TH1D("x integrated", "x integrated", 200, -60, 60);
	TH1D* yintx = new TH1D("y integrated", "y integrated", 200, -60, 60);
	TCanvas *BeamProfileCanvas = new TCanvas("Beam Profile", "Beam Profile (mm)");
	BeamProfileCanvas->Divide(1, 2);
	BeamProfileCanvas->cd(1);
	xinty->Draw();
	BeamProfileCanvas->cd(2);
	yintx->Draw();



	//Scans through all .root files in the folder given by the user
	//parses in all .root files (trees) created by DAQ software
	vector<char*> files;
					
		if (dir != NULL) {
			while (pdir = readdir(dir)) {
				char* fileName = pdir->d_name;
				if (strlen(fileName) > 5 && !strcmp(fileName + strlen(fileName) - 5, ".root")) {

					DataSet* data = new DataSet();
					//using string Folder Name acquire intial tree
					//Initial tree is given by ReMinumber with no underscore number
					//cout << fileName << endl;
					char str[256];
					strcpy(str, fileLocation.c_str());
					strcat(str, fileName);
					//cout << str << endl;
					cout << filenumber << endl;
					//counter for number of files processed
					filenumber++;
					//Loads in the .root tree file
					TFile* rawFile = TFile::Open(str);
					TTree* rawTree = (TTree*)rawFile->Get("T");
					//Takes raw data from tree and sorts it into a dataSet
					//dataSet is the container for all raw and processed data
					loadFromTreeDataSet(rawTree, data);
					rawFile->Close();
					//Associate hits into events, where event is a single particle/ion hit on the detector. Events are sorted by group
					constructEvents(data);
					//Plots any MCP hits time (in ns) for the positive and negative detectors
					plotTimeSpectraDS(data, userDet, &histTimeSpec);

					// construct timesum histograms on first file
					//if electronics are not changed, the timesums should be consistent for the duration of each run. 
					//Therefore only neccasery to acquire timesums for the first tree
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
						checkReconstructable(data, imageUserInfo, minImageTime, maxImageTime);

						//does not need userDet
						DataSet *reconData = sortReconData(data);

						convertLayerPosition(reconData, Pitches, userDet);

						convertCartesianPosition(reconData, userDet, &XYpositions, &UVWlayers);

						//Profiles the beam by integrating along each axis
						for (Group* g : *reconData) {
							for (Event* e : g->events) {
								if (e->mcp->detector == pos) {
									xinty->Fill(e->positive.x);
									yintx->Fill(e->positive.y);
								}
							}
						}

						//Want to image each layer

						for (Group* g : *reconData) {
							for (Event* e : g->events) {
								UVWlayers.UVPoslayers->Fill(e->positive.x_uv, e->positive.y_uv);
								UVWlayers.UWPoslayers->Fill(e->positive.x_uw, e->positive.y_uw);
								UVWlayers.VWPoslayers->Fill(e->positive.x_vw, e->positive.y_vw);
							}
						}




						/**
						xinty->Fit("gaus");
						yintx->Fit("gaus");
						gStyle->SetOptFit(0011);
						TF1 *fitxinty = xinty->GetFunction("gaus");
						TF1 *fityintx = yintx->GetFunction("gaus");
						*/

						//plotTimeSpectraRadius(reconData, userDet, &histTimeSpec);
						/*
						
						*/

						XYPosDet.Modified();
						XYPosDet.Update();
						XYNegDet.Modified();
						XYNegDet.Update();
						UVWNeglayersCanvas.Modified();
						UVWNeglayersCanvas.Update();
						//TimeSpectra->Modified();
						//TimeSpectra->Update();
						BeamProfileCanvas->Modified();
						BeamProfileCanvas->Update();
						UVWNeglayersCanvas.Modified();
						UVWNeglayersCanvas.Update();

						

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

