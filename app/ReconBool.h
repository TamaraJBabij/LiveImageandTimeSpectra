#pragma once
#include "stdafx.h"

//stores a simple bool, does the event have enough layered hits to reconstruct position

enum ReconInfoStruct{
	notReconstructable,
	reconstructable,
	ionNoPosition
};