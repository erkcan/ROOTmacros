// Loop through all histos in the current gPad and make sure all of
// them are totally visible.  Useful when you plot two or more histograms
// with the "same" option and some bin contents are not visible due to
// the range set automatically by the first plotted histo.
void HistoAutoRange() {

  if ( gROOT->GetListOfCanvases()->GetEntries() == 0 ) return;
  TList *glist = gPad->GetListOfPrimitives();

  // First identify the current range for the y-axis
  double ymin=0, ymax=0;
  for (int i=0; i<glist->GetEntries(); ++i) {
    TObject *obj = glist->At(i);
    if ( obj->InheritsFrom("TFrame") ) {
	ymin = ((TFrame*)obj)->GetY1();
	ymax = ((TFrame*)obj)->GetY2();
	break;
    }
  }

  // Now loop through histos and find the extrema
  for (int i=0; i<glist->GetEntries(); ++i) {
    TObject *obj = glist->At(i);
    if ( obj->InheritsFrom("TH1") ) {
      TH1 *histo = (TH1*)obj;
      ymin = TMath::Min( ymin, histo->GetMinimum() );
      ymin = TMath::Min( ymin, histo->GetBinContent(histo->GetMinimumBin()));
      ymax = TMath::Max( ymax, 1.1 * histo->GetBinContent(histo->GetMaximumBin()));
    }
  }

  // Finally set the new min/max
  for (int i=0; i<glist->GetEntries(); ++i) {
    TObject *obj = glist->At(i);
    if ( obj->InheritsFrom("TH1") )
      ((TH1*)obj)->GetYaxis()->SetRangeUser(ymin, ymax);
  }

  gPad->Modified();

}
