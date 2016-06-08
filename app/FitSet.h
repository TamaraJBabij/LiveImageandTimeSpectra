#pragma once
#include "FitData.h"
#include <vector>
#include "Constants.h"
using namespace std;

class FitSet {
private:
	vector<FitData> posFits;
	vector<FitData> negFits;
public:
	FitSet();
	void setFit(Layer layer, Charge charge, double peak, double error, double sigma);
	FitData getFit(Layer layer, Charge charge);
};