
{
	gROOT->ProcessLine(".L Debug/dataTypes.dll");
	gROOT->ProcessLine(".L setup.cfg");
	gROOT->ProcessLine(".include dataTypes");
	gROOT->ProcessLine(".L macros/treeIO.cpp");
	gROOT->ProcessLine(".L macros/plotTimeSpectra.cpp");
	gROOT->ProcessLine(".L macros/ReadWriteTree.cpp");
	gROOT->ProcessLine(".L macros/anuprocess.cpp");
}