// A ROOT macro to plot the same object (like a histogram) from multiple
// root files. For example, say you ran a program multiple times with
// different data samples and created files, each of which contains the 
// histogram "mydir/myhist". Then you can try this:
// root *.root
// .x SameHistoPlotter("mydir/myhist")
// If it is preferable to put the objects into multiple pads instead of
// overlaying them in a single canvas, multipad option can be set.

// One can pass draw options and a single line of ROOT code to be
// processed for each plotted histogram, as the third and fourth
// arguments. (See SameHistoNormPlotter as an example.)

int SameHistoPlotter(TString histoname="", bool multipad=0, TString opt="", const char* cmdPerObj=0)
{
  int noOfPlottedHistos = 0;
  int noOfOpenTFiles = gROOT->GetListOfFiles()->GetEntries();
  // If no canvases around, create a new one.
  if ( gROOT->GetListOfCanvases()->GetEntries() == 0 ) TCanvas::MakeDefCanvas();
  // Note that I use the currently active TPad as my canvas!
  TPad *mycanv = gPad;
  // If multipad option is chosen, we divide the canvas into multiple pads.
  // For simplicity, before we conservatively create one pad for each file that
  // is currently open. (Some pads might therefore remain empty at the end.)
  if ( multipad ) mycanv->Divide(noOfOpenTFiles);
  TLegend *myleg = new TLegend(0.5,0.67,0.88,0.88,histoname.Data());
  for (int k=0; k<noOfOpenTFiles; k++) {
    TFile *myfile = (TFile*) gROOT->GetListOfFiles()->At(k);
    if (histoname!="") {
      if (multipad) mycanv->cd(noOfPlottedHistos+1);
      cout.width(2); cout.fill('0'); // nice formatting of plot number
      cout << noOfPlottedHistos+1 << " - Plotting in ";
      cout.width(7); cout.fill(' '); // nice formatting of color name
      cout << gROOT->GetListOfColors()->At(noOfPlottedHistos+1)->GetName()
	   << ", " << histoname << " from ";
      TObject *myobj = myfile->Get(histoname);
      if ( myobj != 0 ) {
	TString inopt = opt + ((noOfPlottedHistos==0 || multipad) ? "" : "same");
	// To have different colors without sacrificing the flexibility
	// of being independent of particular TObject type, we set the
	// default coloring with gStyle and force it.
	gStyle->SetHistLineColor(noOfPlottedHistos+1);
	gStyle->SetFuncColor(noOfPlottedHistos+1);
	if (cmdPerObj) gROOT->ProcessLineFast(cmdPerObj);
	myobj->UseCurrentStyle();
	myobj->Draw(inopt);
	if (!multipad) myleg->AddEntry(myobj,myfile->GetName(),"lpf");
	noOfPlottedHistos ++;
      }
    }
    // if no histoname is given, just list the currently open files
    else cout << "Currently accessible file ";
    cout << myfile->GetName() << endl;
  }
  if (!multipad && noOfPlottedHistos>0) myleg->Draw();
  return noOfPlottedHistos;
}

// An application of SameHistoPlotter cmdPerObj argument. For TObjects
// that inherit from TH1, histograms are all plotted normalized to unit
// area.
int SameHistoNormPlotter(TString histoname="",
			 bool multipad=0, TString opt="") {
  cout << "Normalizing histograms to unit area.\n";
  return SameHistoPlotter(histoname, multipad, opt, "((TH1*)myobj)->Scale(1./((TH1*)myobj)->GetSumOfWeights());"); }

// Another application of the cmdPerObj argument.  Assuming that the object
// being plotted is a 2D histogram, it draws their x-projections.
int SameHistoXProjectionPlotter(TString histoname="",
				bool multipad=0, TString opt="") {
  return SameHistoPlotter(histoname, multipad, opt, "myobj=new TH1D(* ((TH2*)myobj)->ProjectionX())"); }
