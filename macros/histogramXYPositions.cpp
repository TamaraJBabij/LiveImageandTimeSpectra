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
	XYpositions.positronDET = new TH2D("positronDET", "Positrons", 2000, -60, 60, 2000, -60, 60);
	XYpositions.electronDET = new TH2D("electronDET", "Electrons", 2000, -60, 60, 2000, -60, 60);
	XYpositions.ionDET = new TH2D("ionDET", "Ions", 2000, -60, 60, 2000, -60, 60);

	for (Group* g : *reconData) {
		XYpositions.positronDET->Fill(g->positron.x, g->positron.y);
		XYpositions.electronDET->Fill(g->electron.x, g->electron.y);
		XYpositions.ionDET->Fill(g->ion.x, g->ion.y);
	}
	
	return XYpositions;
}