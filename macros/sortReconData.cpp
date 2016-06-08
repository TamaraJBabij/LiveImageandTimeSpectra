#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "Event.h"
#include "stdafx.h"
#include "configparticletimes.h"
#include <vector>
#include "Constants.h"
#include "MCPTDiff.h"
#include "..\app\macros.h"
#include "TripleCheck.h"

//creates a new dataset with reconstructable data
DataSet* sortReconData(DataSet* data) {
	DataSet* reconData = new DataSet();
	cout << "recon program run" << endl;

	//sort out useful data groups
	for (Group* g : *data) {
		//int check = g->ElecCheck;
		//cout << check << endl;
			 bool recon = true;
			 //cout << " maybe recon group" << endl;
			 for (Event* e : g->events) {
				 if (e->reconstructInfo == notReconstructable) {
					 recon = false;
					 //cout << " not recon group" << endl;
					 break;
				 }
			 }
			 //If the group contains reconstructable particles copy over to new dataset
			 if (recon == true) {
				 //stores a pointer to the group
				 //therefore reconData wiull contains pointers to all relevant groups
				 reconData->addGroup(g);
				 //cout << "recon group" << endl;
			 }
	}
	return reconData;
}