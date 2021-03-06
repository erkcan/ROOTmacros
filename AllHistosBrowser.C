// A ROOT macro to plot all the open histograms one by one.

int AllHistosBrowser(float delay=1.5, TString opt="", TString select="")
{
  int noOfPlottedHistos = 0;
  int noOfBrowsables = gDirectory->GetListOfKeys()->GetEntries();
  TCanvas *mycanvas = new TCanvas("mycanvas");
  for (int k=0; k<noOfBrowsables; k++) {
    TObject *myobj = gROOT->FindObject(gDirectory->GetListOfKeys()->At(k)->GetName());
    if ( myobj != 0 && myobj->InheritsFrom("TH1") ) {
      if ( TString(myobj->GetName()).Index(select)==-1) continue; 
      // if the "same" option is being given, we don't want it to
      // be used for the first histo, otherwise things get ugly
      TString inopt = opt;
      if (noOfPlottedHistos==0) {
	inopt.ToLower(); if (inopt.Index("same")>-1) inopt.ReplaceAll("same",""); }
      if ( gROOT->GetListOfGlobalFunctions()->FindObject("SameHistoPlotter") )
	gROOT->ProcessLineFast("SameHistoPlotter(myobj->GetName());");
      else
	myobj->Draw(inopt.Data());
      mycanvas->Update();
      cout << myobj->GetName() << endl;
      noOfPlottedHistos++;
      TStopwatch wt; while (wt.RealTime()<delay) wt.Continue();
    }
  }
  return noOfPlottedHistos;
}
