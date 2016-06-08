#include <iostream>
#include "DataSet.h"
#include "stdafx.h"
#include "Group.h"
#include "PitchPropSet.h"
#include "PitchData.h"
#include "math.h"

//since we average over all layer reconstructions, want to make sure the numbers for each UV, UW, VW are simliar

void differenceOfLayers(DataSet* reconData) {
	for (Group* g : *reconData) {
		if (g->electron.xy_uv == true && g->electron.xy_uw == true){
			g->electron.x_uv_uw = g->electron.x_uv - g->electron.x_uw;
			g->electron.y_uv_uw = g->electron.y_uv - g->electron.y_uw;
			//cout << "electron y uv " << g->electron.y_uv << endl;
			//cout << "electron y uw " << g->electron.y_uw << endl;
			//cout << "electron y uv - uw " << g->electron.y_uv_uw << endl;
		}
		if (g->electron.xy_uv == true && g->electron.xy_vw == true) {
			g->electron.x_uv_vw = g->electron.x_uv - g->electron.x_vw;
			g->electron.y_uv_vw = g->electron.y_uv - g->electron.y_vw;
		}
		if (g->electron.xy_uw == true && g->electron.xy_vw == true) {
			g->electron.x_uw_vw = g->electron.x_uw - g->electron.x_vw;
			g->electron.y_uw_vw = g->electron.y_uw - g->electron.y_vw;
		}
	}
	
}