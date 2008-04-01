// A macro to find the (n-bin times m-bin) window with highest S/B
// The minimum size of the window and its minimum signal efficiency
// can be set as desired.

// Input parameters:
// signal     = signal TFile name
// background = background TFile name
// histname   = 2D histogram on which the cut window is to be defined
// minxwidth  = minimum number of x-bins in an x-window
// minywidth  = minimum number of y-bins in a y-window
// minsigeff  = minimum desired signal efficiency

void Optimize2D(TString signal, TString background, TString histname,
		int minxwidth = 2, int minywidth = 2, float minsigeff=0.5){

  TFile *sigfile = TFile::Open(signal);
  TFile *bkgfile = TFile::Open(background);

  TH2F *sighist = (TH2F*)sigfile->Get(histname);
  TH2F *bkghist = (TH2F*)bkgfile->Get(histname);

  int nxbins = sighist->GetNbinsX();
  int nybins = sighist->GetNbinsY();
  if ( nxbins != bkghist->GetNbinsX() || nybins != bkghist->GetNbinsY() ) {
    cout << "Error! The two input histograms do not have the same number of bins!" << endl;
    return; }

  int nsigtot = sighist->Integral(1,nxbins,1,nybins);
  int nbkgtot = bkghist->Integral(1,nxbins,1,nybins);
  
  cout << "Total number of entries in the signal histogram: " << nsigtot << endl;
  cout << "Total number of entries in the bkgrnd histogram: " << nbkgtot << endl;

  if ( nsigtot==0 || nbkgtot==0 ) {
    cout << "Error! At least one of the input histograms is empty!" << endl;
    return; }

  TH1D *projx = sighist->ProjectionX("projx");
  TH1D *projy = sighist->ProjectionY("projy");

  float bestsob = (float)nsigtot / nbkgtot;
  cout << "Starting with a \"no-cuts\" s/b of " << bestsob << endl;

  float x1=0, y1=0, x2=0, y2=0; // corner coordinates for the window

  for (int i=1; i<=nxbins; ++i)
    for (int j=minxwidth-1; i+j<=nxbins; ++j) 
      for (int k=1; k<=nybins; ++k)
	for (int l=minywidth-1; k+l<=nybins; ++l) {

	  int nsig = sighist->Integral(i, i+j, k, k+l);
	  if ( nsig == 0 ) continue;
	  if ( nsig < nsigtot*minsigeff ) continue;

	  int nbkg = bkghist->Integral(i, i+j, k, k+l);
	  if ( nbkg == 0 ) continue;

	  float sob = (float)nsig / nbkg;

	  if ( (float)nsig / nbkg > bestsob ) {
	    bestsob = sob;
	    x1 = projx->GetBinLowEdge(i);
	    x2 = projx->GetBinLowEdge(i+j+1);
	    y1 = projy->GetBinLowEdge(k);
	    y2 = projy->GetBinLowEdge(k+l+1);
	    cout << "x-range " << i << " (" << x1
		 << ") to " << i+j << " (" << x2
		 << ") ; \t y-range " << k << " (" << y1
		 <<") to " << k+l << " (" << y2
		 << ") ; \t signal= " << nsig
		 << " bkgrnd= " << nbkg << " s/b= " << sob << endl;
	  }
	  
	}

  TCanvas *cnv = TCanvas::MakeDefCanvas();
  cnv->Divide(2);
  TBox *box = new TBox(x1, y1, x2, y2);
  box->SetFillStyle(0);
  box->SetLineColor(2);
  box->SetLineWidth(2);
  cnv->cd(1); sighist->Draw("colbox"); box->Draw();
  cnv->cd(2); bkghist->Draw("colbox"); box->Draw();
  cnv->SaveAs("Optimize2D"+histname+".eps");

}
