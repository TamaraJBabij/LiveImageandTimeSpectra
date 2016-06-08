#include <iostream>
#include "DataSet.h"
#include "Group.h"
#include "Event.h"
#include "stdafx.h"
#include "HistogramTimeSums.h"
#include "Hit.h"
#include <vector>
#include "configlayers.h"
#include <TFile.h>
#include "FitSet.h"
#include "FitData.h"
#include "LayerHit.h"

using namespace std;

//Checks timesums for each layer
// if timesums not within 2 sigma of peak given by fitting, bad timesums

void checkTimeSums(DataSet* data, FitSet fits) {
	//need to store thresholds
	// 2 sigma for each layer

	//check errors of fits, cout error if error of fits over threshold
	//need to add this in later when more data is put through code
	//error for each layer compared to errormax in configlayers 


	for (Group* g : *data) {
		for (Event* e : g->events) {

			//positive detector layer hits
			if (e->mcp->detector == pos) {
				for (double u1tdiff : e->u1) {
					for (double u2tdiff : e->u2) {
						double timeSum = u1tdiff + u2tdiff;
						FitData fit = fits.getFit(u, positive);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(u, u1tdiff, u2tdiff);
						}
					}
				}
				for (double v1tdiff : e->v1) {
					for (double v2tdiff : e->v2) {
						double timeSum = v1tdiff + v2tdiff;
						FitData fit = fits.getFit(v, positive);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(v, v1tdiff, v2tdiff);
						}
					}
				}
				for (double w1tdiff : e->w1) {
					for (double w2tdiff : e->w2) {
						double timeSum = w1tdiff + w2tdiff;
						FitData fit = fits.getFit(w, positive);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(w, w1tdiff, w2tdiff);
						}
					}
				}
			}
		}
	}
	for (Group* g : *data) {
		for (Event* e : g->events) {
			//positive detector layer hits
			if (e->mcp->detector == neg) {
				for (double u1tdiff : e->u1) {
					for (double u2tdiff : e->u2) {
						double timeSum = u1tdiff + u2tdiff;
						FitData fit = fits.getFit(u, negative);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(u, u1tdiff, u2tdiff);
						}
					}
				}
				for (double v1tdiff : e->v1) {
					for (double v2tdiff : e->v2) {
						double timeSum = v1tdiff + v2tdiff;
						FitData fit = fits.getFit(v, negative);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(v, v1tdiff, v2tdiff);
						}
					}
				}
				for (double w1tdiff : e->w1) {
					for (double w2tdiff : e->w2) {
						double timeSum = w1tdiff + w2tdiff;
						FitData fit = fits.getFit(w, negative);
						double tdiff = timeSum - fit.peak;
						double threshold = 2 * fit.sigma;
						//At this point it should check against the thhreshold for the timesum
						//compares timesum for layer against the fitted timesum 2sigma
						//if within this +/- 2 sigma then the layer times will be stored in a paired object 
						if (tdiff < threshold && tdiff > -threshold) {
							//put timesums that are within 2 sigma (threshold) of the fit
							// into paired layers struct
							e->storePair(w, w1tdiff, w2tdiff);
						}
					}
				}
			}
		}
	}
};