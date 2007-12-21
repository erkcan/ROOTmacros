//TH1D inhisto("myhisto","Good histo", 100, 0, 500)

int combineBins(TH1F inhisto, float xmin, float xmax)
{
  int startbin = inhisto.FindBin(xmin);
  int endbin = inhisto.FindBin(xmax);

  if (startbin >= endbin) return 0;

  cout << "Combining bins " << startbin << " to " << endbin << endl;
  const int newNbins = inhisto.GetNbinsX() - endbin + startbin;

  Double_t edgeArr[newNbins+1];

  for (int i=0; i<=newNbins; ++i) {
    int skipbin = 0;
    if (i>=startbin) skipbin = endbin - startbin;
    edgeArr[i] = inhisto.GetBinLowEdge(i+1+skipbin);
  }

  TH1F *newhist = new TH1F("newhist","newhist",newNbins,edgeArr);

  for (int i=0; i<=newNbins+1; ++i) {
    int skipbin = 0;
    if (i>=startbin) skipbin = endbin - startbin;
    newhist->SetBinContent(i, inhisto.GetBinContent(i+skipbin));
    newhist->SetBinError(i, inhisto.GetBinError(i+skipbin));
  }


  /*for (int i=1; i<=newhist->GetNbinsX()+1; ++i)
    cout << newhist->GetBinLowEdge(i) << "\t";
    cout << endl;*/

  return newNbins;
}


// Function to remove bins from the beginning or the end of a TH1F
TH1F* CropBins(TH1F *inhisto, float nbinCropMin=0, int nbinCropMax=0) {

  if ( nbinCropMin * nbinCropMax < 0 || nbinCropMin+nbinCropMax==0 )
    return inhisto;

  const int newNbins = inhisto->GetNbinsX() - nbinCropMin - nbinCropMax;
  if ( newNbins < 1 ) return inhisto;

  float newXmin = inhisto->GetXaxis()->GetBinLowEdge(nbinCropMin+1);
  float newXmax = inhisto->GetXaxis()->GetBinUpEdge(newNbins+nbinCropMin);

  TH1F *newhist = new TH1F("newhist",inhisto->GetTitle(),newNbins,newXmin,newXmax);
  newhist->GetYaxis()->SetTitle(inhisto->GetYaxis()->GetTitle());
  newhist->GetXaxis()->SetTitle(inhisto->GetXaxis()->GetTitle());

  float newUnderflow = 0;
  for (int i=0; i<=nbinCropMin; ++i) {
    newUnderflow += inhisto->GetBinContent(i); }
  newhist->SetBinContent(0, newUnderflow);

  for (int i=1; i<newNbins; ++i) {
    newhist->SetBinContent(i, inhisto->GetBinContent(i+nbinCropMin));
    if ( inhisto->GetBinError(i+nbinCropMin) )
      newhist->SetBinError(i, inhisto->GetBinError(i+nbinCropMin));
  }

  float newOverflow = 0;
  for (int i=newNbins+nbinCropMin; i<=inhisto->GetNbinsX(); ++i) {
    newOverflow += inhisto->GetBinContent(i); }
  newhist->SetBinContent(newNbins+1, newOverflow);

  return newhist;
}
