//file that tells root what custom c++ classes we want to use in root
#ifndef __CINT__
#pragma link C++ class DataSet;
#pragma link C++ class std::vector<Group*>+;
#pragma link C++ class std::vector<Hit>+;
#pragma link C++ class Group;
#endif