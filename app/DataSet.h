//makes sure compiler only processes once
#ifndef DATASET_H
#define DATASET_H

#include "Group.h"
#include <vector>

//Object to load from tree, store all groups/hits and write back to tree
class DataSet {
private:
	//Set of groups. Each group contains some data and a set of hits, which contain channel and time info
	vector<Group*> data;
public:
	//constructor, code that runs when object is first made. Sets the initial state of the object
	DataSet();
	//destructor, code that runs when an object is deleted
	~DataSet();

	void addGroup(Group* g);


	// iterator is a generic way to access any data structure that c defines, special class in c++ that is like a pointer to some data
	// lets the complier know that the class is a container that holds Groups*
	// allows for easy looping through Group
	//means datasets act like a collection of groups
	vector<Group*>::iterator begin();
	vector<Group*>::iterator end();

	vector<Group*>* getGroups();
};
#endif