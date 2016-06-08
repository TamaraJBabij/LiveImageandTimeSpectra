#pragma once 
#include "stdafx.h"

//layer hit object - store layer hits with appropriate timesums
//appropriate timesum is defined as nsigma within peak as fitted by gaus

struct LayerHit {
	double line1;
	double line2;
};