#include "stdafx.h"
#include "PitchPropSet.h"
#include <vector>
#include "Constants.h"

void PitchPropSet::setPitchProp(Charge charge, double ulayer, double vlayer, double wlayer) {
	PitchPropData data;
	data.ulayer = ulayer;
	data.vlayer = vlayer;
	data.wlayer = wlayer;

	if (charge == positive) {
		posPitch = data;
	}
	else {
		negPitch = data;
	}
}

PitchPropData PitchPropSet::getPitchProp(Charge charge) {
	if (charge == positive) {
		return posPitch;
	}
	else {
		return negPitch;
	}
}