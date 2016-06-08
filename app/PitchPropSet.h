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
	void setPitchProp(Charge charge, double ulayer, double vlayer, double wlayer);
	PitchPropData getPitchProp(Charge charge);
};
