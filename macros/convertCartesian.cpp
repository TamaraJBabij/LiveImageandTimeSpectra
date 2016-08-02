#include <iostream>
#include "DataSet.h"
#include "stdafx.h"
#include "Group.h"
#include "PitchPropSet.h"
#include "PitchData.h"
#include "math.h"
#include "Constants.h"
#include "HistogramXY.h"
#include "histogramElecLayers.h"
#include "calibrateLayersHist.h"

void convertCartesianPosition(DataSet* reconData, imagingDetectors userDet, HistogramXY* XYpositions = NULL, HistogramElecLayers* UVWlayers = NULL, calibrateLayersHist* UVWMasklayers = NULL) {
	if (userDet == bothDet) {
		for (Group* g : *reconData) {
			//cout << "Group:" << endl;
			for (Event* e : g->events) {
				if (e->mcp->detector == pos) {
					Particle p;
					p.t = e->reltimediff.timediff;
					p.x = 0;
					p.y = 0;
					p.xy_uv = false;
					p.xy_uw = false;
					p.xy_vw = false;
					int count = 0;
					if (e->uPairs.size() == 1 && e->vPairs.size() == 1) {
						//g->positron = Particle(32,23123,2341)
						p.x_uv = e->U;
						p.y_uv = (1.0 / sqrt(3))*(e->U - 2 * e->V);
						p.x += p.x_uv;
						p.y += p.y_uv;
						count++;
						p.xy_uv = true;
						//cout << "UV: " << p.y_uv << endl;
					}
					else if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_uw = e->U;
						p.y_uw = (1.0 / sqrt(3))*(2 * e->W - e->U);
						p.x += p.x_uw;
						p.y += p.y_uw;
						count++;
						p.xy_uw = true;
						//cout << "UW: " << p.y_uw << endl;
					}
					else if (e->vPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_vw = e->V + e->W;
						p.y_vw = (1.0 / sqrt(3))*(e->W - e->V);
						p.x += p.x_vw;
						p.y += p.y_vw;
						count++;
						p.xy_vw = true;
					}
					p.x = p.x / count;
					p.y = p.y / count;
					e->positive = p;
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
						p.y_uv = (1.0 / sqrt(3))*(-e->U + 2 * e->V);
						p.y += p.y_uv;
						count++;
						p.xy_uv = true;
					}
					if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_uw = e->U;
						p.y_uw = (1.0 / sqrt(3))*(2 * e->W + e->U);
						p.x += p.x_uw;
						p.y += p.y_uw;
						count++;
						p.xy_uw = true;
					}
					if (e->vPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_vw = e->V + e->W;
						p.y_vw = (1.0 / sqrt(3))*(e->W + e->V);
						p.x += p.x_vw;
						p.y += p.y_vw;
						count++;
						p.xy_vw = true;
					}
					p.x = p.x / count;
					p.y = p.y / count;
					//cout << "x " << p.x << endl;
					//cout << "y " << p.y << endl;
					e->negative = p;
				}
			}
		}
	}
	else if (userDet == posDet) {
		for (Group* g : *reconData) {
			//cout << "Group:" << endl;
			for (Event* e : g->events) {
				if (e->mcp->detector == pos) {
					Particle p;
					p.t = e->reltimediff.timediff;
					p.x = 0;
					p.y = 0;
					p.xy_uv = false;
					p.xy_uw = false;
					p.xy_vw = false;
					int count = 0;
					if (e->uPairs.size() == 1 && e->vPairs.size() == 1) {
						//g->positron = Particle(32,23123,2341)
						p.x_uv = e->U;
						//p.x_uv = -e->U;
						p.y_uv = (1.0 / sqrt(3))*(e->U - 2 * e->V);
						//p.y_uv = (1.0 / sqrt(3))*( e->U - 2 * e->V);
						p.x += p.x_uv;
						p.y += p.y_uv;
						//cout << "Xuv: " << p.x_uv << endl;
						//cout << "Yuv: " << p.y_uv << endl;
						count++;
						p.xy_uv = true;
					}
					else if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_uw = e->U;
						//p.x_uw = -e->U;
						p.y_uw = (1.0 / sqrt(3))*(2 * e->W - e->U);
						//p.y_uw = (1.0 / sqrt(3))*(2 * e->W - e->U);
						p.x += p.x_uw;
						p.y += p.y_uw;
						count++;
						p.xy_uw = true;
					}
					else if (e->vPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_vw = e->W + e->V;
						//p.x_vw = - e->W - e->V; //v+w
						p.y_vw = (1.0 / sqrt(3))*(e->W - e->V);
						//p.y_vw = (1.0 / sqrt(3))*(e->W - e->V);
						p.x += p.x_vw;
						p.y += p.y_vw;
						count++;
						p.xy_vw = true;
					}
					p.x = p.x / count;
					p.y = p.y / count;
					e->positive = p;
					if (UVWlayers != NULL) {
						if (userDet == negDet) {
							UVWlayers->UVlayers->Fill(e->negative.x_uv, e->negative.y_uv);
							UVWlayers->UWlayers->Fill(e->negative.x_uw, e->negative.y_uw);
							UVWlayers->VWlayers->Fill(e->negative.x_vw, e->negative.y_vw);
						}
						if (userDet == posDet) {
							UVWlayers->UVPoslayers->Fill(e->positive.x_uv, e->positive.y_uv);
							UVWlayers->UWPoslayers->Fill(e->positive.x_uw, e->positive.y_uw);
							UVWlayers->VWPoslayers->Fill(e->positive.x_vw, e->positive.y_vw);
						}
					}
					if (XYpositions != NULL) {
						if (userDet == negDet) {
							XYpositions->electronDET->Fill(e->negative.x, e->negative.y);
							if (e->negative.xy_uv == true) {
								if (e->negative.y_uv<60 && e->negative.y_uv>-60) {
									UVWMasklayers->UVMasklayer->Fill(e->negative.x_uv);
								}
							}
							if (e->negative.xy_uw == true) {
								if (e->negative.y_uw<60 && e->negative.y_uw>-60) {
									UVWMasklayers->UWMasklayer->Fill(e->negative.x_uw);
								}
							}
							if (e->negative.xy_vw == true) {
								if (e->negative.y_vw<60 && e->negative.y_vw> -60) {
									UVWMasklayers->VWMasklayer->Fill(e->negative.x_vw);
								}
							}
							if (e->negative.xy_uv == true) {
								if (e->negative.x_uv<60 && e->negative.x_uv>-60) {
									UVWMasklayers->UVMasklayerY->Fill(e->negative.y_uv);
								}
							}
							if (e->negative.xy_uw == true) {
								if (e->negative.x_uw<60 && e->negative.x_uw>-60) {
									UVWMasklayers->UWMasklayerY->Fill(e->negative.y_uw);
								}
							}
							if (e->negative.xy_vw == true) {
								if (e->negative.x_vw<60 && e->negative.x_vw> -60) {
									UVWMasklayers->VWMasklayerY->Fill(e->negative.y_vw);
								}
							}
						}
						if (userDet == posDet) {
							XYpositions->positronDET->Fill(e->positive.x, e->positive.y);
							//cout << e->positive.x << " " << e->positive.y << endl;
				
						}
					}
				}
			}
		}
	}
	else if (userDet == negDet) {
		for (Group* g : *reconData) {
			//cout << "Group:" << endl;
			for (Event* e : g->events) {
				if (e->mcp->detector == neg) {
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
						p.y_uv = (1 / sqrt(3))*(-e->U + 2 * e->V);
						p.y += p.y_uv;
						count++;
						p.xy_uv = true;
					}
					if (e->uPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_uw = e->U;
						p.y_uw = (1 / sqrt(3))*(2 * e->W + e->U);
						p.x += p.x_uw;
						p.y += p.y_uw;
						count++;
						p.xy_uw = true;
					}
					if (e->vPairs.size() == 1 && e->wPairs.size() == 1) {
						p.x_vw = e->V - e->W;
						p.y_vw = (1 / sqrt(3))*(e->W + e->V);
						p.x += p.x_vw;
						p.y += p.y_vw;
						count++;
						p.xy_vw = true;
						
					}
					p.x = p.x / count;
					p.y = p.y / count;
					//cout << "x " << p.x << endl;
					//cout << "y " << p.y << endl;
					e->negative = p;
					if (UVWlayers != NULL) {
						if (userDet == negDet) {
							UVWlayers->UVlayers->Fill(e->negative.x_uv, e->negative.y_uv);
							UVWlayers->UWlayers->Fill(e->negative.x_uw, e->negative.y_uw);
							UVWlayers->VWlayers->Fill(e->negative.x_vw, e->negative.y_vw);
						}
						if (userDet == posDet) {
							UVWlayers->UVPoslayers->Fill(e->positive.x_uv, e->positive.y_uv);
							UVWlayers->UWPoslayers->Fill(e->positive.x_uw, e->positive.y_uw);
							UVWlayers->VWPoslayers->Fill(e->positive.x_vw, e->positive.y_vw);
						}
					}
					if (XYpositions != NULL) {
						if (userDet == negDet) {
							XYpositions->electronDET->Fill(e->negative.x, e->negative.y);
							if (e->negative.xy_uv == true) {
								if (e->negative.y_uv<60 && e->negative.y_uv>-60) {
									UVWMasklayers->UVMasklayer->Fill(e->negative.x_uv);
								}
							}
							if (e->negative.xy_uw == true) {
								if (e->negative.y_uw<60 && e->negative.y_uw>-60) {
									UVWMasklayers->UWMasklayer->Fill(e->negative.x_uw);
								}
							}
							if (e->negative.xy_vw == true) {
								if (e->negative.y_vw<60 && e->negative.y_vw> -60) {
									UVWMasklayers->VWMasklayer->Fill(e->negative.x_vw);
								}
							}
							if (e->negative.xy_uv == true) {
								if (e->negative.x_uv<60 && e->negative.x_uv>-60) {
									UVWMasklayers->UVMasklayerY->Fill(e->negative.y_uv);
								}
							}
							if (e->negative.xy_uw == true) {
								if (e->negative.x_uw<60 && e->negative.x_uw>-60) {
									UVWMasklayers->UWMasklayerY->Fill(e->negative.y_uw);
								}
							}
							if (e->negative.xy_vw == true) {
								if (e->negative.x_vw<60 && e->negative.x_vw> -60) {
									UVWMasklayers->VWMasklayerY->Fill(e->negative.y_vw);
								}
							}
						}
						if (userDet == posDet) {
							XYpositions->positronDET->Fill(e->positive.x, e->positive.y);
							//cout << e->positive.x << " " << e->positive.y << endl;
							if (e->positive.xy_uv == true) {
								if (e->positive.y_uv<60 && e->positive.y_uv>-60) {
									UVWMasklayers->UVPosMasklayer->Fill(e->positive.x_uv);
								}
							}
							if (e->positive.xy_uw == true) {
								if (e->positive.y_uw<60 && e->positive.y_uw>-60) {
									UVWMasklayers->UWPosMasklayer->Fill(e->positive.x_uw);
								}
							}
							if (e->positive.xy_vw == true) {
								if (e->positive.y_vw<60 && e->positive.y_vw> -60) {
									UVWMasklayers->VWPosMasklayer->Fill(e->positive.x_vw);
								}
							}
							if (e->positive.xy_uv == true) {
								if (e->positive.x_uv<60 && e->positive.x_uv>-60) {
									UVWMasklayers->UVPosMasklayerY->Fill(e->positive.y_uv);
								}
							}
							if (e->positive.xy_uw == true) {
								if (e->positive.x_uw<60 && e->positive.x_uw>-60) {
									UVWMasklayers->UWPosMasklayerY->Fill(e->positive.y_uw);
								}
							}
							if (e->positive.xy_vw == true) {
								if (e->positive.x_vw<60 && e->positive.x_vw> -60) {
									UVWMasklayers->VWPosMasklayerY->Fill(e->positive.y_vw);
								}
							}
						}
					}
				}
			}
		}
	}
	/*
	else {
		cout << "no detector detected in convert cartesian" << endl;
	}
	*/
}