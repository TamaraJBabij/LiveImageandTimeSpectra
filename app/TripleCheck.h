#pragma once

//stores check values for particles
//if other value greater than zero nto a good group
//ideally have 1 pos, 1 ion, 1 elec, otherwise bad group

struct TripleCheck
{
	double PosCheck;
	double ElecCheck;
	double Ion1Check;
	double Ion2Check;
	double UnidentCheck;
	bool TripleCoinc;
};