#include "DataSet.h"
#include "Constants.h"
#include "histlayerSums.h"
#include "TH1D.h"

void calibrateFromSlope(DataSet* reconData, imagingDetectors userDet, layerDiffHist* diffHist) {
	//make a graph of udiffs against expected x, fit a linear tendline and produce a slope equation, use that for pitch prop and offset

	if (userDet == negDet) {
		for (Group* g : *reconData) {
			for (Event* e : g->events) {
				if (e->mcp->detector == neg) {
					if (e->uPairs.size() == 1) {
						double uDiff = (e->uPairs.front().line1 - e->uPairs.front().line2);
						diffHist->uNegLayerDiff->Fill(uDiff);
					}
					if (e->vPairs.size() == 1) {
						double vDiff = (e->vPairs.front().line1 - e->vPairs.front().line2);
						//diffHist->vNegLayerDiff->Fill(vDiff);
						if (e->negative.x_uv < (e->negative.y_uv + 2.00) && e->negative.x_uv >(e->negative.y_uv - 2.00)) {
							diffHist->vNegLayerDiff->Fill(vDiff);
						}
					}
					if (e->wPairs.size() == 1) {
						double wDiff = (e->wPairs.front().line1 - e->wPairs.front().line2);
						//diffHist->wNegLayerDiff->Fill(wDiff);
						if (e->negative.x_uw < (e->negative.y_uw - 2.00) && e->negative.x_uv >(e->negative.y_uw + 2.00)) {
							diffHist->wNegLayerDiff->Fill(wDiff);
						}
					}
				}
			}
		}
	}
}