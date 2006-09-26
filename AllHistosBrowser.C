// A ROOT macro to plot all the open histograms one by one.

int AllHistosBrowser(float delay=1.5, TString opt="")
{
  int noOfPlottedHistos = 0;
  int noOfBrowsables = gDirectory->GetListOfKeys()->GetEntries();
  TCanvas *mycanvas = new TCanvas("mycanvas");
  cout << "Please press enter to go to the next plot one after the other.\n";
  for (int k=0; k<noOfBrowsables; k++) {
    TObject *myobj = gROOT->FindObject(gDirectory->GetListOfKeys()->At(k)->GetName());
    if ( myobj != 0 && myobj->InheritsFrom("TH1") ) {
      myobj->Draw(opt.Data());
      mycanvas->Update();
      cout << myobj->GetName() << endl;
      noOfPlottedHistos++;
      TStopwatch wt; while (wt.RealTime()<delay) wt.Continue();
    }
  }
  return noOfPlottedHistos;
}
