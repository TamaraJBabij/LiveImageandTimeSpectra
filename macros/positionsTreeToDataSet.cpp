#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "TTree.h"
#include "configuration.h"
#include "TFile.h"

//make each group, fill it with hits, add it to the dataset
void positionsTreeToDataSet(TTree* tree, DataSet* data, imagingDetectors userDet) {
	Double_t u1Time, u2Time, v1Time, v2Time, w1Time, w2Time; 
	bool ULayer, VLayer, WLayer;
	Int_t GroupNumber; 
	Hit* h = new Hit();
	//Set up the tree to store values from each entry of the raw data tree
	tree->SetBranchAddress("GroupNumber", &GroupNumber);
	tree->SetBranchAddress("ULayer", &ULayer);
	tree->SetBranchAddress("u1Time", &u1Time);
	tree->SetBranchAddress("u2Time", &u2Time);
	tree->SetBranchAddress("VLayer", &VLayer);
	tree->SetBranchAddress("v1Time", &v1Time);
	tree->SetBranchAddress("v2Time", &v2Time);
	tree->SetBranchAddress("WLayer", &WLayer);
	tree->SetBranchAddress("w1Time", &w1Time);
	tree->SetBranchAddress("w2Time", &w2Time);

	//Establish data container variables
	Group* currentGroup = NULL;

	int N = (int)tree->GetEntries();

	//This will loop through all of the entries in the tree
	for (int i = 0; i < N; i++) {
		tree->GetEntry(i);
		
		if (currentGroup == NULL) {
			//remember the first group
			currentGroup = new Group(GroupNumber);
		}

		else if (GroupNumber != currentGroup->getId()) {
			data->addGroup(currentGroup);
			currentGroup = new Group(GroupNumber);
		}
		
		if (userDet == posDet) {
			//Hit* h = new Hit();
			h->channel = ChannelID::mcp;
			h->detector = DetectorID::pos;
			h->time = 0;
			currentGroup->addHit(h);
		}

		if (userDet == negDet) {
			//Hit* h = new Hit();
			h->channel = ChannelID::mcp;
			h->detector = DetectorID::neg;
			h->time = 0;
			currentGroup->addHit(h);
		}

		
		
		Event* e = new Event(h);
		e->reltimediff.timediff = 0;
		//make the Hit object by loading the values from the tree according to the config file
			if (ULayer != 0){
				e->storePair(u, u1Time, u2Time);
				//cout << "U Layer" << endl;
			}
			if (VLayer != 0) {
				e->storePair(v, v1Time, v2Time);
				//cout << "V Layer" << endl;
			}
			if (WLayer != 0) {
				e->storePair(w, w1Time, w2Time);
				//cout << "W Layer" << endl;
			}
			currentGroup->addEvent(e);
			//cout << "event added" << endl;
	}
}

/*FIRST ENTRY: make a new group and remember group id
NEW ID: store current group into dataset, make a new group and remember group ID
SAME ID: nothing

ALL 3: add hit to current group*/