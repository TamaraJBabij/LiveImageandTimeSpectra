#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "TTree.h"
#include "configuration.h"
#include "TFile.h"

// Root does histogram DataSet as there is poor vector intergration
// will try writing data to a new tree with time converted from comSTOP mode and in ns from bins. 
// Will then attempt to histogram

TTree* readWriteTree(TTree* tree) {
	//READ TREE
	//define the variables to hold the read values
	int rawGroupNumber, rawChannel, rawBins;
	//Set up the tree to store values from each entry of the raw data tree
	// Tell tree to populate these variables when reading an entry
	//tree->SetBranchAddress("Name of Column", &Set name of pointer);
	tree->SetBranchAddress("GroupNumber", &rawGroupNumber);
	tree->SetBranchAddress("Channel", &rawChannel);
	tree->SetBranchAddress("Time", &rawBins);

	//WRITE TREE
	//Establish data container variables
	// Data container in this case will be another tree
	//create a tree file tree1.root - create the file, the Tree and a few branches
	TFile file("DataSetTree.root", "recreate");
	TTree treeDS("treeDS", "simple tree that stores raw data");
	Int_t groupNumber, channel;
	Double_t time;
	treeDS.Branch("GroupNumber", &groupNumber);
	treeDS.Branch("Channel", &channel);
	treeDS.Branch("Time", &time);

	//FILL TREE
	int N = (int)tree->GetEntries();

	//This will loop through all of the entries in the tree
	for (int i = 0; i < N; i++) {
		tree->GetEntry(i);

		//make the new dataset tree by loading the values from the tree according to the config file
		time = 32000 - 0.5*rawBins;
		switch (rawChannel) {
			case CFG_CHANNEL_POS_CP2:
				channel = rawChannel;
				time -= CFG_DELAY_POS_CP2;
				break;
			case CFG_CHANNEL_POS_U1_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_U1_S;
				break;
			case CFG_CHANNEL_POS_U2_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_U2_S;
				break;
			case CFG_CHANNEL_POS_V1_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_V1_S;
				break;
			case CFG_CHANNEL_POS_V2_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_V2_S;
				break;
			case CFG_CHANNEL_POS_W1_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_W1_S;
				break;
			case CFG_CHANNEL_POS_W2_S:
				channel = rawChannel;
				time -= CFG_DELAY_POS_W2_S;
				break;
			case CFG_CHANNEL_ELEC_CP2:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_CP2;
				break;
			case CFG_CHANNEL_ELEC_U1_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_U1_S;
				break;
			case CFG_CHANNEL_ELEC_U2_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_U2_S;
				break;
			case CFG_CHANNEL_ELEC_V1_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_V1_S;
				break;
			case CFG_CHANNEL_ELEC_V2_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_V2_S;
				break;
			case CFG_CHANNEL_ELEC_W1_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_W1_S;
				break;
			case CFG_CHANNEL_ELEC_W2_S:
				channel = rawChannel;
				time -= CFG_DELAY_ELEC_W2_S;
				break;
			}
			groupNumber = rawGroupNumber;
			treeDS.Fill();
			
		}
		treeDS.Write();
		return &treeDS;
		
}

/*FIRST ENTRY: make a new group and remember group id
NEW ID: store current group into dataset, make a new group and remember group ID
SAME ID: nothing

ALL 3: add hit to current group*/