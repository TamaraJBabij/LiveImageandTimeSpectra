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

HistogramXY histogramXYPositions(DataSet *reconData) {
	//sort for particle type using 
	//Particle is subset of event and contains time and x, y
	HistogramXY XYpositions;
	XYpositions.positronDET = new TH2D("positronDET", "Positrons", 400, -60, 60, 400, -60, 60);
	XYpositions.electronDET = new TH2D("electronDET", "Electrons", 400, -60, 60, 400, -60, 60);

	for (Group* g : *reconData) {
		//cout << "x " << g->positive.x << endl;
		if (g->positive.x > 0.08 || g->positive.x < -0.01) {
			if (g->positive.y > 0.08 || g->positive.y < -0.08) {
				XYpositions.positronDET->Fill(g->positive.x, g->positive.y);
			}
		}
		if (g->negative.x > 0.08 || g->negative.x < -0.01) {
			if (g->negative.y > 0.08 || g->negative.y < -0.08) {
				XYpositions.electronDET->Fill(g->negative.x, g->negative.y);
			}
		}
		//XYpositions.positronDET->Fill(g->positive.x, g->positive.y);
		//XYpositions.electronDET->Fill(g->negative.x, g->negative.y);
	}
	
	return XYpositions;
}