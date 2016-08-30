#pragma once 
#include "stdafx.h"
#include <vector>
#include "Hit.h"
#include "LayerHit.h"
#include "Constants.h"
#include "MCPTDiff.h"
#include "ReconBool.h"
#include "Particle.h"


//stores the associated layer hits for the mcp hit 
//layer hit object - store layer hits with appropriate timesums
//appropriate timesum is defined as nsigma within peak as fitted by gaus


class Event
{
private:


public:
	Event(Hit* mcp);
	void storeHitInfo(double tdiff, ChannelID channel);
	void storePair(Layer layer, double line1, double line2);
	void storeMCPTDiff(ParticleID particle, double timediff);
	//As recon info public
	//void Event::storeReconInfo(bool reconstructable);

	Hit* mcp;
	vector<double> u1;
	vector<double> u2;
	vector<double> v1;
	vector<double> v2;
	vector<double> w1;
	vector<double> w2;
	//time relative to positron
	// for positron hits this will be zero  
	//assuming positron is first hit on the detector
	//vector<MCPTDiff> pos;
	//vector<MCPTDiff> ion;
	//vector<MCPTDiff> other;
	//vector<MCPTDiff> elec;
	vector<LayerHit> uPairs;
	vector<LayerHit> vPairs;
	vector<LayerHit> wPairs;
	
	ReconInfoStruct reconstructInfo;

	MCPTDiff reltimediff;

	//implemented to test if issue with MCPTDiff
	ParticleID particletype;
	double timefrompos;

	// layers
	double U;
	double V;
	double W;

	//If user wants to image a certain timing window. 
	//Bool is for whether event is in timing window (true) or not in timing window (false)
	bool imageHit;

	Particle positive;
	Particle negative;

};
