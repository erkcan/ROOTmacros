// A ROOT macro that reads in numbers from a given ASCII file,
// fills them into a TTree and plots them on a histogram.

// Just fill the TTree and use ROOT's own TTree->Draw()
int PlotASCII(TString filename) { return PlotASCII(filename,0,0); }

// xmin and xmax are the limits of the histogram to be created.
// Any value read outside [xmin,xmax] range will become under/overflow.
int PlotASCII(TString filename, float xmin, float xmax)
{
  TTree *mytree = new TTree(filename,filename+" ROOT tree");
  float x;
  mytree->Branch(filename,&x,filename);

  ifstream asciiFile(filename);
  int sayac = 0;
 
  while (asciiFile >> x)
    {
      mytree->Fill();
      sayac++;
    }

  TH1F *myhisto;
  if (xmin!=xmax) myhisto = new TH1F(filename+"_histo",filename+"_histo", 100, xmin, xmax);
  else myhisto = new TH1F();
  mytree->Draw(filename+">>"+filename+"_histo");

  return sayac;
}
