#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <string>
#include "Hit.h"
//used to make objects compatible with root
#include "TObject.h" 


using namespace std;
/*
	Enum for each possible particle type
*/
enum ParticleID
{
	ParticleID_unknown,
	ParticleID_positron,
	ParticleID_electron,
	ParticleID_ion1,
	ParticleID_ion2
};

/*
	Reconstructed particle with position and time data
*/
struct Particle
{
	double x;
	double y;
	double t;
};

/*
	Contains information on several hits within 500ns of
	an MCP Hit
*/
struct Set
{
	ParticleID id;// = ParticleID_unknown;
	vector<Hit> v_mcp, v_x1, v_x2, v_y1, v_y2, v_z1, v_z2;
	Hit mcp, x1, x2, y1, y2, z1, z2;
};


class Group : public TObject {
private:
    //Numerical id for group generated by TDC
    int id;

	//Final particle information
	Particle electron, positron, ion;

	//Raw data of Hits from file
	vector<Hit> rawData;

	//Set
	vector<Set>	setData;
public:
	//Constructor - do everything you need to when you create a Group object
	Group(int id);
	Group();

	//Destructor - do everything you need to when you destroy a Group object
	~Group();

	void addHit(Hit h);

	int getId();

	vector<Hit>* getHits();

	//required for root to use the object
	ClassDef(Group, 1);
};
#endif
