#include "stdafx.h"
#include <iostream>
#include <TH1.h>
#include <TF1.h>
#include <TH2D.h>
#include "HistogramXY.h"
#include "DataSet.h"
#include "Group.h"

// takes the X,Y positions and makes a 2D histogram for each detector
// hist for positrons, ions and electrons separately

void histogramXYPositions(DataSet *reconData, HistogramXY XYpositions) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	

	for (Group* g : *reconData) {
		for (Event* e : g->events) {
			//cout << "x " << e->positive.x << endl;
			if (e->positive.x) {
				if (e->positive.y) {
					XYpositions.positronDET->Fill(e->positive.x, e->positive.y);
				}
			}
			if (e->negative.x) {
				if (e->negative.y) {
					XYpositions.electronDET->Fill(e->negative.x, e->negative.y);
					cout << "hist fill" << endl;
				}
			}
			//XYpositions.positronDET->Fill(e->positive.x, e->positive.y);
			//XYpositions.electronDET->Fill(e->negative.x, e->negative.y);
		}
	}
	
}