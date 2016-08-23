#pragma once
#include <vector>

using namespace std;

enum Layer {
	u,
	v,
	w
};

enum Charge {
	positive,
	negative
};

enum imagingDetectors {
	posDet = 'pos',
	negDet = 'neg',
	bothDet = 'both'
};

enum commandInfo {
	imageInfo = 'imag',
	timesumInfo = 'time',
	bothInfo = 'both'

};

/*
Enum for each possible particle type
*/
enum ParticleID
{

	positron,
	electron,
	ion1,
	ion2,
	unidentified
};

enum CalibrateLoadType {
	PositionTreeWrite,
	PositionTreeRead,
	PositionTreeCreate,
	PositionTreeErrorCalibrate,
	PositionTreeSlope,
	PositionTreeIterate
};

struct Constants {
	static const vector<Layer> layers;
	static const vector<Charge> charges;
};