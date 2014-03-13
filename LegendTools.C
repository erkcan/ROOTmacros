// A macro to print out a TLegend - can be considered a smarter TLegend::ls().
// If no TLegend pointer is passed, it loops over the TLegends drawn on current TPad.
void PrintLegend(TLegend *leg=0) {
  if ( leg==0 ) {
    if (gROOT->GetListOfCanvases()->GetEntries()==0) return;
    TList *padprim = gPad->GetListOfPrimitives();
    for (int i=0; i<padprim->GetEntries(); i++) {
      TObject *myobj = gROOT->FindObject(padprim->At(i)->GetName());
      if ( myobj != 0 && myobj->InheritsFrom("TLegend") )
	PrintLegend((TLegend*)myobj); }
    return;
  }
  TList *ents = leg->GetListOfPrimitives();
  for (int i=0; i<ents->GetEntries(); i++) {
    TLegendEntry *le = (TLegendEntry*)ents->At(i);
    TString s( le->GetLabel() );
    TObject *obj = le->GetObject();
    if (!obj) continue;  // if no object, this can be the title line, so skip
    TString color = "???";
    if ( obj->InheritsFrom("TH1") )
      color = gROOT->GetListOfColors()->At(((TH1*)obj)->GetLineColor())->GetName();
    cout << "Item ";  cout.width(2); cout.fill('0'); cout << i << " plotted in ";
    cout.width(7); cout.fill(' '); cout << color << " : " << s << endl;
  }
  //leg->ls();
}
