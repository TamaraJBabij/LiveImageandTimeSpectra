#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Hit.h"
#include "Event.h"
#include "stdafx.h"
#include <vector>
#include "Constants.h"
#include "MCPTDiff.h"
#include "..\app\macros.h"

//selects out events with a pos, ion and elec 
//(still need to make sure elec and pos have enough info to reconstruct position)
//if group has more than 1 pos, elec or ion no good
// if group has any other unident particles then no good
 
void selectPosIonElec(DataSet* data) {
	// want to index number of particles and bool triple for each each group
	//Should first check that each event has enough layer hits to be reconstructed 
	//if ion then set bool to true automatically, otherwise check layer hits
	for (Group* g : *data) {
		int PosCheck = 0;
		int ElecCheck = 0;
		int Ion1Check = 0;
		int Ion2Check = 0;
		int UnidentCheck = 0;
		bool TripleCoinc;
		for (Event* e : g->events) {
			//index particle ids
			//previous switch (e.reltimediff.particle)
			switch (e->particletype) {
			case positron:
				PosCheck++; 
				//cout << "positron identified" << endl;
				break;
			case electron:
				ElecCheck++;
				break;
			case ion1:
				Ion1Check++;
				break;
			case ion2:
				Ion2Check++;
				break;
			case unidentified:
				UnidentCheck++;
				break;
			}
		}
		if (PosCheck == 1 && ElecCheck == 1 && Ion1Check == 1 && Ion2Check == 0 && UnidentCheck == 0) {
			TripleCoinc = true;
			//cout << "triple coinc true " << PosCheck << ElecCheck<< Ion1Check<< Ion2Check << UnidentCheck << endl;
			g->PosCheck = PosCheck;
			g->ElecCheck = ElecCheck;
			g->Ion1Check = Ion1Check;
			g->Ion2Check = Ion2Check;
			g->UnidentCheck = UnidentCheck;
			g->TripleCoinc = TripleCoinc;
		}
		else {
			TripleCoinc = false;
			//cout << "triple coinc false " << PosCheck << ElecCheck << Ion1Check << Ion2Check << UnidentCheck << endl;
			g->PosCheck = PosCheck;
			g->ElecCheck = ElecCheck;
			g->Ion1Check = Ion1Check;
			g->Ion2Check = Ion2Check;
			g->UnidentCheck = UnidentCheck;
			g->TripleCoinc = TripleCoinc;
		}
		//g->storeTripleCheck(PosCheck, ElecCheck, Ion1Check, Ion2Check, UnidentCheck, TripleCoinc);
	}
}

