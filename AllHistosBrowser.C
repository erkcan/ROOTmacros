// A ROOT macro to plot all the open histograms one by one.

int AllHistosBrowser(long delay=2e6)
{
  int noOfPlottedHistos = 0;
  int noOfBrowsables = gDirectory->GetListOfKeys()->GetEntries();
  TCanvas *mycanvas = new TCanvas("mycanvas");
  cout << "Please press enter to go to the next plot one after the other.\n";
  for (int k=0; k<noOfBrowsables; k++) {
    TObject *myobj = gROOT->FindObject(gDirectory->GetListOfKeys()->At(k)->GetName());
    if ( myobj != 0 && myobj->InheritsFrom("TH1") ) {
      myobj->Draw();
      mycanvas->Update();
      cout << myobj->GetName() << endl;
      noOfPlottedHistos++;
      for (long w=0; w<delay; w++) continue;
    }
  }
  return noOfPlottedHistos;
}
