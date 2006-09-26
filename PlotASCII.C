// A ROOT macro that reads in numbers from a given ASCII file,
// fills them into a TTree.

int PlotASCII(TString filename)
{
  TFile *myfile = new TFile("plotascii.root","RECREATE",filename+" ROOT file");
  TTree *mytree = new TTree(filename,filename+" ROOT tree");
  double x;
  mytree->Branch(filename,&x,filename);

  ifstream asciiFile(filename);
  int sayac = 0;
 
  while (asciiFile >> x)
    {
      mytree->Fill();
      sayac++;
    }

  mytree->Draw();
  return sayac;
}
