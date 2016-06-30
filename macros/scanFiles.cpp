#include <iostream>
#include "dirent.h"
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "TTree.h"
#include "configuration.h"
#include "TFile.h"
#include "macros.h"
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

//scans folder and loads all tree ( .root) files into the dataset
DataSet* scanFiles() {
	
	string fileLocation;
	cout << "what is the directory? remember to add a backslash at the end" << endl;
	cin >> fileLocation;
	DataSet* data = new DataSet();

	DIR* dir;
	dirent* pdir;
	dir = opendir(fileLocation.c_str());

	vector<char*> files;
	if (dir != NULL) {
		while (pdir = readdir(dir)) {
			char* fileName = pdir->d_name;
			if (strlen(fileName) > 5 && !strcmp(fileName + strlen(fileName) - 5, ".root")) {
				//using string Folder Name acquire intiial tree
				//Initial tree is given by ReMinumber with no underscore number
				cout << fileName << endl;
				char str[256];
				strcpy (str, fileLocation.c_str());
				strcat(str, fileName);
				cout << str << endl;
				TFile* rawFile = TFile::Open(str);
				TTree* rawTree = (TTree*)rawFile->Get("T");
				loadFromTreeDataSet(rawTree, data);
				rawFile->Close();
				
			}
		}
	}
	else {
		cout << "Directory does not exist, exiting" << endl;
		exit(1);
	}

	closedir(dir);

	return data;
}