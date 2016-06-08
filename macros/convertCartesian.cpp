#include <iostream>
#include "DataSet.h"
#include "stdafx.h"
#include "Group.h"
#include "PitchPropSet.h"
#include "PitchData.h"
#include "math.h"

void convertCartesianPosition(DataSet* reconData) {
	//cout << "convertCartesianPosition" << endl;
	for (Group* g : *reconData) {
		//cout << "Group:" << endl;
		for (Event* e : g->events) {
			if (e->mcp->detector == pos) {
				Particle p;
				p.t = e->reltimediff.timediff;
				if (e->uPairs.size() == 1 && e->vPairs.size() == 1) {
					//g->positron = Particle(32,23123,2341)
					p.x = e->U;
					p.y = (1 / sqrt(3))*(e->U - 2 * e->V);
				}
				else if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
					p.x = e->U;
					p.y = (1 / sqrt(3))*(2 * e->W - e->U);
				}
				else {
					p.x = e->V + e->W;
					p.y = (1 / sqrt(3))*(e->W - e->V);
				}
				switch (e->reltimediff.particle) {
				case positron:
					g->positron = p;
					//cout << " - positron" << " x: " << p.x << " y: " << p.y << endl;
					break;
					//case electron:
						//g->electron = p;
						//cout << " - electron" << " x: " << p.x << " y: " << p.y << endl;
						//break;
				case ion1:
					g->ion = p;
					//cout << " - ion" << " x: " << p.x << " y: " << p.y << endl;
					break;
				}
			}
			else if (e->mcp->detector == neg) {
				Particle p;
				p.t = e->reltimediff.timediff;
				p.x = 0;
				p.y = 0;
				//setting these bools allows for layers to be plotted separately 
				//as the corresponding doubles are set up in the group.h they exist
				//makes hard to sort for UV and VW and UW constructions
				//bools tagged reconstructed events
				p.xy_uv = false;
				p.xy_uw = false;
				p.xy_vw = false;
				int count = 0;
				if (e->uPairs.size() == 1 && e->vPairs.size() == 1) {
					//g->positron = Particle(32,23123,2341)
					//see logbook 9 page 125-126 for why this cartesian conversion is used. 
					//see logbook 9 page 114 for diagram of electron detector
					p.x_uv = e->U;
					p.x += p.x_uv;
					p.y_uv = (1 / sqrt(3))*( - e->U + 2 * e->V);
					p.y += p.y_uv;
					count++;
					p.xy_uv = true;
				}
				if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
					p.x_uw = e->U;
					p.y_uw = (1 / sqrt(3))*( 2 * e->W + e->U);
					p.x += p.x_uw;
					p.y += p.y_uw;
					count++;
					p.xy_uw = true;
   				}
				if (e->vPairs.size() == 1 && e->wPairs.size() == 1) {
					p.x_vw = e->V - e->W;
					p.y_vw = (1 / sqrt(3))*( e->W + e->V);
					p.x += p.x_vw;
					p.y += p.y_vw;
					count++;
					p.xy_vw = true;
				}
				p.x = p.x / count;
				p.y = p.y / count;
				g->electron = p;
			}
		}
	}
}