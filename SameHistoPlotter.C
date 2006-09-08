// A ROOT macro to plot the same object (like a histogram) from multiple
// root files. For example, say you ran a program multiple times with
// different data samples and created files, each of which contains the 
// histogram "mydir/myhist". Then you can try this:
// root *.root
// .x SameHistoPlotter("mydir/myhist")
// If it is preferable to put the objects into multiple pads instead of
// overlaying them in a single canvas, multipad option can be set.
int SameHistoPlotter(TString histoname="", bool multipad=0)
{
  int noOfPlottedHistos = 0;
  for (int k=0; k<gROOT->GetListOfFiles()->GetEntries(); k++) {
    TFile *myfile = (TFile*) gROOT->GetListOfFiles()->At(k);
    if (histoname!="") { 
      cout << "Plotting in "
	   << gROOT->GetListOfColors()->At(noOfPlottedHistos+1)->GetName()
	   << ", " << histoname << " from ";
      if ( myfile->Get(histoname) != 0 ) {
	TString opt = noOfPlottedHistos==0 ? "" : "same";
	// To have different colors without sacrificing the flexibility
	// of being independent of particular TObject type, we set the
	// default coloring with gStyle and force it.
	gStyle->SetHistLineColor(noOfPlottedHistos+1);
	gStyle->SetFuncColor(noOfPlottedHistos+1);
	gROOT->ForceStyle();
	myfile->Get(histoname)->Draw(opt);
	noOfPlottedHistos ++; }
    }
    // if no histoname is given, just list the currently open files
    else cout << "Currently accessible file ";
    cout << myfile->GetName() << endl;
  }
  return noOfPlottedHistos;
}
