#include "stdafx.h"
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include "TFunction.h"
#include "TH2D.h"
#include "constants.h"
#include "HistogramXY.h"
#include "TMath.h"
#include "TF2.h"

/*Creates a tree of X,Y positions corresponding to the calibraiton mask on either 
the positive or the negative detector*/

void createMaskDataTree(imagingDetectors userDet, HistogramXY* XYpositions, CalibrateLoadType sessionOption) {
	if (userDet == negDet) {
		/*creates data for the negative detector UV, UW and WV layers, accounting for the gaps in the detector*/
		//double y = 0;
		//double x = 0;
		//check brackets
		TF2 *f2 = new TF2("2Dgaus", "exp(-1.0*((pow(x-0,2)/(2*pow(1,2)))+(pow(y-0,2)/(2*pow(1,2)))))", -5, 5, -5, 5);
		XYpositions->modeldataNegDET->FillRandom("2Dgaus", 10000);
	}
}