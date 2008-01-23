// A simple macro to generate a generic TLegend on the
//  active pad. The user can simply modify it later...
{
  TList *prim = gPad->GetListOfPrimitives();
  TLegend *genLeg = new TLegend(0.6,0.7,0.89,0.89);

  for ( int i=0; i<prim->GetSize(); ++i) {

    TObject *padObj = prim->At(i);
    if ( padObj->InheritsFrom("TH1F") )
      genLeg->AddEntry(padObj, padObj->GetName());

      }

  genLeg->Draw();

}
