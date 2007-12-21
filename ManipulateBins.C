/* Testing combineBins
.L ManipulateBins.C
TH1F myhisto("myhisto","Good histo", 20, 0, 100);
for (int i=1;i<21;++i) myhisto.SetBinContent(i,32);
TH1F *newh = CombineBins(myhisto,21,26);
newh->SetLineColor(4);
myhisto.Draw()
newh->Draw("same");
*/

// A function to make a new histogram by combining the contents of selected
// consecutive bins in an input histogram
TH1F* CombineBins(TH1F inhisto, float xmin, float xmax)
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

  TH1F *newhist = new TH1F("newhist",inhisto.GetTitle(),newNbins,edgeArr);
  newhist->GetYaxis()->SetTitle(inhisto.GetYaxis()->GetTitle());
  newhist->GetXaxis()->SetTitle(inhisto.GetXaxis()->GetTitle());

  // Starting from the new underflow bin, up to the new overflow bin,
  // copy contents for the "untouched" bins. For the new "combination"
  // bin, copy only the startbin
  for (int i=0; i<=newNbins+1; ++i) {
    int skipbin = 0;
    if (i>startbin) skipbin = endbin - startbin;
    newhist->SetBinContent(i, inhisto.GetBinContent(i+skipbin));
    newhist->SetBinError(i, inhisto.GetBinError(i+skipbin));
  }

  // Now we have to handle the "combination" bin. We need to add
  // everything from startbin+1 to endbin. 
  for (int i=startbin+1; i<=endbin; ++i) {
    newhist->SetBinContent(startbin, newhist->GetBinContent(startbin)
			   +inhisto.GetBinContent(i));
    newhist->SetBinError(startbin, sqrt(pow(newhist->GetBinError(startbin),2)+
					pow(inhisto.GetBinError(i),2)));
  }
  /*for (int i=1; i<=newhist->GetNbinsX()+1; ++i)
    cout << newhist->GetBinLowEdge(i) << "\t";
    cout << endl;*/

  return newhist;
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
