// Loop through all histos in the current gPad and put them into separate
// pads on a new Canvas. The maximum and the minimum is kept at the same
// level on each of the pads.
void HistosToMultiPad(TString drawopt="", TCanvas *h2mpCanvas=0) {

  if ( gROOT->GetListOfCanvases()->GetEntries() == 0 ) return;
  TList *glist = gPad->GetListOfPrimitives();

  int nhistos(0);
  double histomax(0), histomin(0);

  // Now loop through histos and find the extrema
  for (int i=0; i<glist->GetEntries(); ++i) {
    TObject *obj = glist->At(i);
    if ( obj->InheritsFrom("TH1") ) {
      TH1 *histo = (TH1*)obj;
      nhistos++;
      if ( histomax == histomin ) {
	histomax = histo->GetMaximum();
	histomin = histo->GetMinimum();
      } else {
	histomax = TMath::Max( histomax, histo->GetMaximum() );
	histomin = TMath::Min( histomin, histo->GetMinimum() );
      }
    }
  }

  if ( h2mpCanvas == 0 )
    h2mpCanvas = new TCanvas("h2mpCanvas","h2mpCanvas");
  h2mpCanvas->Divide(nhistos);

  nhistos = 0;
  for (int i=0; i<glist->GetEntries(); ++i) {
    TObject *obj = glist->At(i);
    if ( obj->InheritsFrom("TH1") ) {
      h2mpCanvas->cd(++nhistos);
      TH1 *histo = (TH1*)obj;
      cout << "Pad " << nhistos << " - " << histo->GetName() << endl;
      if ( histo->InheritsFrom("TH2") )
	histo->GetZaxis()->SetRangeUser(histomin,histomax*1.1);
      else
	histo->GetYaxis()->SetRangeUser(histomin,histomax*1.1);
      TH1* copyhisto = histo->DrawCopy(drawopt);
      copyhisto->UseCurrentStyle();
    }
  }

  h2mpCanvas->Modified();

}
