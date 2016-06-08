#include "stdafx.h"
#include "FitSet.h"
#include <vector>
#include "Constants.h"

FitSet::FitSet() {
	posFits = vector<FitData>(3, FitData());
	negFits = vector<FitData>(3, FitData());
}

void FitSet::setFit(Layer layer, Charge charge, double peak, double error, double sigma) {
	FitData data;
	data.peak = peak;
	data.error = error;
	data.sigma = sigma;

	if (charge == positive) {
		posFits[layer] = data;
	}
	else {
		negFits[layer] = data;
	}
}

FitData FitSet::getFit(Layer layer, Charge charge) {
	if (charge == positive) {
		return posFits.at(layer);
	}
	else {
		return negFits.at(layer);
	}
}