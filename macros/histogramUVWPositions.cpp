#include "stdafx.h"
#include <iostream>
#include <TH1.h>
#include <TF1.h>
#include <TH1D.h>
#include "HistUVWPositions.h"
#include "DataSet.h"
#include "Group.h"

// takes the X,Y positions and makes a 2D histogram for each detector
// hist for positrons, ions and electrons separately

void histogramUVWPositions(DataSet *reconData, UVWpositionsHist *UVWPositions) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	cout << "Hist U V and W program run" << endl;
	for (Group* g : *reconData) {
		for (Event* e : g->events) {
			if (e->U == true) {
				UVWPositions->UNegPositions->Fill(e->U);
				cout << "U layer: " << e->U << endl;
			}
			if (e->V == true) {
				UVWPositions->VNegPositions->Fill(e->V);
			}
			if (e->W == true) {
				UVWPositions->VNegPositions->Fill(e->W);
			}
		}
	}
}

