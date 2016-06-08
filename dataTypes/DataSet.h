//makes sure compiler only processes once
#ifndef DATASET_H
#define DATASET_H

#include "Group.h"
#include <vector>
//used to make objects compatible with root
#include "TObject.h" 

//Object to load from tree, store all groups/hits and write back to tree
class DataSet : public TObject{
private:
	//Set of groups. Each group contains some data and a set of hits, which contain channel and time info
	vector<Group*> data;
public:
	//constructor, code that runs when object is first made. Sets the initial state of the object
	DataSet();
	//destructor, code that runs when an object is deleted
	~DataSet();

	void addGroup(Group* g);

	vector<Group*>* getGroups();
	//required for root to use the object
	ClassDef(DataSet, 1);
};
#endif