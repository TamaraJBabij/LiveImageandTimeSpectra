// Read data from a parsed ascii file and create a root file with a Tree.
   
TFile *anubuild(Int_t get=0, Int_t print=1) {

   Int_t           GroupNumber;
   Int_t           Channel;
   Int_t           Time;
   

   //Loads the file parsed.tsv and stores a pointer in fp
   TString filename = "parsed.tsv";
   TString dir = @"H:\2015\9-10-2015, 1157\";
   FILE *fp = fopen(Form("%s%s",dir.Data(),filename.Data()),"r");

   TFile *hfile = 0;
   if (get) {
      // if the argument get =1 return the file "cernstaff.root"
      // if the file does not exist, it is created
      if (!gSystem->AccessPathName(dir+"parsed.root",kFileExists)) {
         hfile = TFile::Open(dir+"parsed.root"); //in $ROOTSYS/tutorials/tree
         if (hfile) return hfile;
      }
      //otherwise try $PWD/cernstaff.root
      if (!gSystem->AccessPathName("parsed.root",kFileExists)) {
         hfile = TFile::Open("parsed.root"); //in current dir
         if (hfile) return hfile;
      }
   }
   //no cernstaff.root file found. Must generate it !
   //generate cernstaff.root in $ROOTSYS/tutorials/tree if we have write access
   if (!gSystem->AccessPathName(dir,kWritePermission)) {
      filename = dir+"parsed.root";
   } else if (!gSystem->AccessPathName(".",kWritePermission)) {
      //otherwise generate cernstaff.root in the current directory
   } else {
      printf("you must run the script in a directory with write access\n");
      return 0;
   }
   hfile = TFile::Open(filename,"RECREATE");
   TTree *tree = new TTree("T","Parsed raw data from LeCroy 3377 TDC");
   tree->Branch("GroupNumber",&GroupNumber,"GroupNumber/I");
   tree->Branch("Channel",&Channel,"Channel/I");
   tree->Branch("Time",&Time,"Time/I");

   char line[80];
   while (fgets(&line,80,fp)) {
      sscanf(&line[0],"%d %d %d",&GroupNumber,&Channel,&Time);
      tree->Fill();
   }
   if (print) tree->Print();
   tree->Write();

   fclose(fp);
   delete hfile;
   if (get) {
      //we come here when the script is executed outside $ROOTSYS/tutorials/tree
      hfile = TFile::Open(filename);
      return hfile;
   }
   return 0;
}