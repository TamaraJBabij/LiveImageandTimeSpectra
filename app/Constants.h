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

struct Constants {
	static const vector<Layer> layers;
	static const vector<Charge> charges;
};