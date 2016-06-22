#include <iostream>
#include "DataSet.h"
#include "stdafx.h"
#include "Group.h"
#include "PitchPropSet.h"
#include "PitchData.h"
#include "math.h"

//converts from timing u1 u2 to U
//uses pitch propogation factor to convert timing info to position info
//stills need to be converted to cartesian after this
void convertLayerPosition(DataSet* reconData, PitchPropSet Pitches, imagingDetectors userDet) {
	//retrieve pitch propagation calculations
	//these are same for all events/groups
	PitchPropData posPitches = Pitches.getPitchProp(positive);
	PitchPropData negPitches = Pitches.getPitchProp(negative);

	if (userDet == bothDet) {
		for (Group* g : *reconData) {
			for (Event* e : g->events) {
				if (e->mcp->detector == pos) {
					if (e->uPairs.size() == 1) {
						e->U = (posPitches.ulayer / 2)*(e->uPairs.front().line1 - e->uPairs.front().line2);
						//cout << e->uPairs.front().line1 << endl;
						//cout << "U layer: " << e->U << endl;
					}
					if (e->vPairs.size() == 1) {
						e->V = (posPitches.vlayer / 2)*(e->vPairs.front().line1 - e->vPairs.front().line2);
						//cout << "V layer: " << e->V << endl;
					}
					if (e->wPairs.size() == 1) {
						e->W = (posPitches.wlayer / 2)*(e->wPairs.front().line1 - e->wPairs.front().line2);
						//cout << "W layer: " << e->W << endl;
					}
				}
				if (e->mcp->detector == neg) {
					//see logbook 9 page 114 for diagram of electron detector
					//including gap sizes
					if (e->uPairs.size() == 1) {
						//line2 - line 1 as u1 and u2 reversed (u2 on right hand side) x = righthandside-lefthandside
						double Unogap = (negPitches.ulayer / 2)*(e->uPairs.front().line1 - e->uPairs.front().line2);
						//cout << "U layer: " << Unogap << endl;
						if (Unogap < 1) {
							e->U = Unogap - (9 / 2);
						}
						else {
							e->U = Unogap + (9 / 2);
						}
					}
					if (e->vPairs.size() == 1) {
						double Vnogap = (negPitches.vlayer / 2)*(e->vPairs.front().line1 - e->vPairs.front().line2);
						//cout << "V layer: " << Vnogap << endl;
						if (Vnogap < 0) {
							e->V = Vnogap - (8 / 2);
						}
						else {
							e->V = Vnogap + (8 / 2);
						}
					}
					if (e->wPairs.size() == 1) {
						//line2 0 lin1 due to electronics configuration
						double Wnogap = (negPitches.wlayer / 2)*(e->wPairs.front().line1 - e->wPairs.front().line2);
						//cout << "W layer: " << Wnogap << endl;
						if (Wnogap < 0.5) {
							e->W = Wnogap - (8 / 2);
						}
						else {
							e->W = Wnogap + (8 / 2);
						}
					}
				}
			}
		}
	}
	else if (userDet == pos) {
		for (Group* g : *reconData) {
			for (Event* e : g->events) {
				if (e->mcp->detector == pos) {
					if (e->uPairs.size() == 1) {
						e->U = (posPitches.ulayer / 2)*(e->uPairs.front().line1 - e->uPairs.front().line2);
						//cout << e->uPairs.front().line1 << endl;
						//cout << "U layer: " << e->U << endl;
					}
					if (e->vPairs.size() == 1) {
						e->V = (posPitches.vlayer / 2)*(e->vPairs.front().line1 - e->vPairs.front().line2);
						//cout << "V layer: " << e->V << endl;
					}
					if (e->wPairs.size() == 1) {
						e->W = (posPitches.wlayer / 2)*(e->wPairs.front().line1 - e->wPairs.front().line2);
						//cout << "W layer: " << e->W << endl;
					}
				}
			}
		}
	}
	else {
		//det must be negative
		for (Group* g : *reconData) {
			for (Event* e : g->events) {
				if (e->mcp->detector == neg) {
					//see logbook 9 page 114 for diagram of electron detector
					//including gap sizes
					if (e->uPairs.size() == 1) {
						//line2 - line 1 as u1 and u2 reversed (u2 on right hand side) x = righthandside-lefthandside
						double Unogap = (negPitches.ulayer / 2)*(e->uPairs.front().line1 - e->uPairs.front().line2);
						//cout << "U layer: " << Unogap << endl;
						if (Unogap < 1) {
							e->U = Unogap - (9 / 2);
						}
						else {
							e->U = Unogap + (9 / 2);
						}
					}
					if (e->vPairs.size() == 1) {
						double Vnogap = (negPitches.vlayer / 2)*(e->vPairs.front().line1 - e->vPairs.front().line2);
						//cout << "V layer: " << Vnogap << endl;
						if (Vnogap < 0) {
							e->V = Vnogap - (8 / 2);
						}
						else {
							e->V = Vnogap + (8 / 2);
						}
					}
					if (e->wPairs.size() == 1) {
						//line2 0 lin1 due to electronics configuration
						double Wnogap = (negPitches.wlayer / 2)*(e->wPairs.front().line1 - e->wPairs.front().line2);
						//cout << "W layer: " << Wnogap << endl;
						if (Wnogap < 0.5) {
							e->W = Wnogap - (8 / 2);
						}
						else {
							e->W = Wnogap + (8 / 2);
						}
					}
				}
			}
		}
	}
}