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
  TTree *mytree = new TTree(filename+"_tree",filename+" ROOT tree");
  float x;
  mytree->Branch("input",&x,filename);

  ifstream asciiFile(filename);
  int sayac = 0;

  char dummy[999];

  while ( !asciiFile.eof() )
    {
      for ( int i=1; i<colno; i++ ) asciiFile >> dummy;
      if ( ! (asciiFile >> x) ) break;
      //cout << x << endl;
      mytree->Fill();
      sayac++;
      asciiFile.getline(dummy, 999);
    }

  TH1F *myhisto;
  if (xmin!=xmax) myhisto = new TH1F(filename+"_histo",filename+"_histo", 100, xmin, xmax);
  else myhisto = new TH1F();
  mytree->Draw("input>>"+filename+"_histo","",drawOpt);

  return sayac;
}
