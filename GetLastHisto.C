TH1* GetLastHisto()
{
  if (!gPad) return 0;
  TObject *obj = gPad->GetListOfPrimitives()->Last();
  while (obj) {
    if (obj->InheritsFrom("TH1")) return (TH1*)obj;
    obj = gPad->GetListOfPrimitives()->Before(obj);
  }
  return 0;
}
