//Stores hits and relevant information into a group, group identified by group number
//Raw data goes in, group comes out, you cant explain that

#include "stdafx.h"
#include <vector>
#include <string>
#include "Hit.h"
#include "Group.h"
#include "TObject.h" 
#include "Event.h"


//constructor where id is passed in as group number from raw tree
Group::Group(int _id)
{
    id = _id;
}
Group::Group(){}
//destructor
Group::~Group() {
	for (Hit* h : rawData) {
		delete h;
	}
	for (Event* e : events) {
		delete e;
	}
}

//gets ID of current group
int Group::getId(){
	return id;
}

//stores a hit object in the set of raw data.
//parsing in tree of data, and storing that in a vector of hits in each group
void Group::addHit(Hit* h)
{
	rawData.push_back(h);
}

//stores an event object in the set of groups.

void Group::addEvent(Event* e)
{
	events.push_back(e);
}



//as defined in header file (see comments for explanation)
// when using an object as a data container end and begin functions must exist (in c++)
// allows looping through the dataset which loops through the groups in the vector
vector<Hit*>::iterator Group::begin() {
	return rawData.begin();
}
vector<Hit*>::iterator Group::end() {
	return rawData.end();
}

vector<Hit*>* Group::getHits(){
	return &rawData;
}

void Group::storeTripleCheck(double PosCheck, double ElecCheck, double Ion1Check, double Ion2Check, double UnidentCheck, bool TripleCoinc) {
	//store number of particle types and whether group fits triple coincidence criteria 
	TripleCheck particles;
	particles.PosCheck = PosCheck;
	particles.ElecCheck = ElecCheck;
	particles.Ion1Check = Ion1Check;
	particles.Ion2Check = Ion2Check;
	particles.UnidentCheck = UnidentCheck;
	particles.TripleCoinc = TripleCoinc;
}