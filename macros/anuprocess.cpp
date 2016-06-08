#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "HistogramPair.h"
#include "TTree.h"
#include <vector>


void anuprocess()
{
	TCanvas *c1 = new TCanvas("c1", "Histograms", 1000, 700);
	gStyle->SetOptStat(0);
	c1->Divide(1, 1, 0, 0);


	TTree *thisGroup = T->CopyTree("GroupNumber==0"); //Copy the group == X tree into a new tree called group
	Int_t GroupNumber; //Vairables to hold info
	Int_t Channel;
	Int_t Time;
	thisGroup->SetBranchAddress("GroupNumber", &GroupNumber);
	thisGroup->SetBranchAddress("Time", &Time);  
	thisGroup->SetBranchAddress("Channel", &Channel);
	std::vector<int> PCP(0);
	PCP.reserve(10);
	PCP.clear();
	std::vector<int> ECP(0);
	ECP.reserve(10);
	ECP.clear();
	thisGroup->BuildIndex("Time");
	TTreeIndex *index = (TTreeIndex*)thisGroup->GetTreeIndex();
	for (int i = index->GetN() - 1; i >= 0; --i) {
		thisGroup->GetEntry(index->GetIndex()[i]);
		if (Channel == 16)
			PCP.push_back(Time);
		if (Channel == 24)
			ECP.push_back(Time);
	}
	cout << "Number of positive hits : " << PCP.size() << " number of negtive hits : " << ECP.size() << endl;

	//Lets get some fitting
	c1->cd(2);
	//The >>> pipes the histogram into hpx vairable then it's reinit.
	T->Draw("Time>>hpx", "Channel==16&&Time>0&&Time<60200"); //Histogram to look at average times.
	TH1F *hpx = (TH1F*)gROOT->FindObject("hpx");
	TF1 *myfit = new TF1("myfit", function, 59700, 60200, 2); //fit the function below to the given range.
	myfit->SetParameters(59900, 30);
	myfit->SetParNames("mu", "sigma", "h");

	//normalize the histogram so the area under the curve = 1	
	hpx->Scale((1 / hpx->Integral()));
	hpx->Fit(myfit);


	//TH1F *h1 = (TH1F*)hpx->Clone("h1"); //Subtract first Gaussian
	//h1->Add(h1->GetFunction("myfit"),-1);

	//h1->Fit(myfit2);

	//get fitting parameters 
	//double a =  myfit->GetParameter(1);
	//cout<<a<<endl;

	//c1->cd(1);
	//hpx->Draw();

	c1->cd(1);
	//h1->Draw();

	TH1F *h2 = hpx->Rebin(1, "h2");
	h2->Scale((1 / h2->Integral()));
	TF1 *myfit2 = new TF1("myfit2", function, 59700, 60200, 2);
	myfit2->SetParameters(60000, 50);
	myfit2->SetParNames("mu", "sigma", "h");
	h2->Fit(myfit2);
	h2->Draw();
	printf("Integral %f \n\n\n", h2->Integral());
	printf("Integral %f \n\n\n", myfit2->Integral(59700, 60200));
}

Double_t function(Double_t* x, Double_t* p)
{
	//Fit a normal distribution
	Double_t factor = (x[0] - p[0]) / p[1];
	Double_t multi = 1 / (p[1] * 2.5066);
	Double_t result = multi*TMath::Exp(-0.5*factor*factor);
	return result;
}