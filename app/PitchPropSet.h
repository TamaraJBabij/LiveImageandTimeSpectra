#pragma once
#include "PitchData.h"
#include <vector>
#include "Constants.h"

//struct to store the pitch propogation values to be used
//to get X Y position info

using namespace std;

class PitchPropSet {
private:
	PitchPropData posPitch;
	PitchPropData negPitch;
public:
	void setPitchProp(Charge charge, double uPitchProp, double uOffset, double vPitchProp, double vOffset, double wPitchProp, double wOffset);
	void setPitchProp(Charge charge, PitchPropData data);
	PitchPropData getPitchProp(Charge charge);
};
