// A ROOT macro that reads in numbers from a given ASCII file,
// fills them into a TTree and plots them on a histogram.

// It is possible to choose a particular column (or "field" in
//   awk jargon) of data on each line.

// Just fill the TTree and use ROOT's own TTree->Draw()
int PlotASCII(TString filename, int colno=1, TString drawOpt="") {
  return PlotASCII(filename,0,0,colno,drawOpt); }

// xmin and xmax are the limits of the histogram to be created.
// Any value read outside [xmin,xmax] range will become under/overflow.
int PlotASCII(TString filename, float xmin, float xmax,
	      int colno=1, TString drawOpt="")
{
  TString treename = filename; treename.ReplaceAll("/","_");
  TTree *mytree = new TTree(treename+"_tree",treename+" ROOT tree");
  float x;
  mytree->Branch("input",&x,treename);

  ifstream asciiFile(filename);
  int sayac = 0;

  char dummy[999];

  while ( !asciiFile.eof() )
    {
      for ( int i=1; i<colno; i++ ) asciiFile >> dummy;
      if ( ! (asciiFile >> dummy) ) break;
      x = atof(dummy);
      //cout << x << endl;
      mytree->Fill();
      sayac++;
      asciiFile.getline(dummy, 999);
    }

  TH1F *myhisto;
  if (xmin!=xmax) myhisto = new TH1F(treename+"_histo",treename+"_histo", 100, xmin, xmax);
  else myhisto = new TH1F();
  mytree->Draw("input>>"+treename+"_histo","",drawOpt);

  return sayac;
}
