// A list of fitting related tools...

// A function to return the chi2, ndof and chi2 probability
// for the last fit made to a histogram.  When a histogram is
// fit, ROOT by default prints out the values of the fitted
// parameters, but writes the chi2/ndof only into the fit
// box on the canvas.  This function allows the user to easily
// print out the chi2 information.

// If no histogram pointer is passed, PrintLastFit checks if
// the current TPad has any histograms with fitted functions.

TString PrintLastFit(TH1 *fittedHisto=0)
{
  TString out = "";
  if (fittedHisto == 0) {
    if (gROOT->GetListOfCanvases()->GetEntries()==0) return out;
    TList *padprim = gPad->GetListOfPrimitives();
    for (int i=0; i<padprim->GetEntries(); i++) {
      TObject *myobj = gROOT->FindObject(padprim->At(i)->GetName());
      // We are looking for histograms on the current pad
      if ( myobj != 0 && myobj->InheritsFrom("TH1") ) {
	cout << ((TH1*)myobj)->GetListOfFunctions()->GetEntries() << endl;
	// We only want histograms which have functions assigned to them.
	// However saved histograms seem to have a TPaveText as the last
	// "function" associated with them. So we check for TF1 inheritance.
	if ( ((TH1*)myobj)->GetListOfFunctions()->GetEntries() == 0 ||
	    !((TH1*)myobj)->GetListOfFunctions()->First()->InheritsFrom("TF1"))
	  continue;
	fittedHisto=(TH1*)myobj; break; }
    }
    if (fittedHisto == 0) return out;
  }

  if (fittedHisto->GetListOfFunctions()->GetEntries() == 0) return out;
  // We now have a histogram which has some functions associated with it.
  // We will start from the last entry from the list and move back one by
  // one until we find an object that inherits from TF1.
  for ( TObject *iobj=fittedHisto->GetListOfFunctions()->Last();
	iobj!=0; iobj=fittedHisto->GetListOfFunctions()->Before(iobj) )
    if ( iobj->InheritsFrom("TF1") ) {
      TF1 *fitfunc = (TF1*)iobj;
      if (fitfunc == 0) return out;
      out += fitfunc->GetName();
      out += "\t : chi2 / ndof = ";
      out += fitfunc->GetChisquare(); out += " / ";
      out += fitfunc->GetNDF(); out += "\t (Prob=";
      out += fitfunc->GetProb(); out += ")";
      break; }
  return out;
}


// A function to fit a histogram in split ranges.  Pass to it the
// histogram that you would like to be fit, a TArray of the bins over
// which the fitting is to be done.  For example:
/* TH1F h("h","test",100,-2,2); h.FillRandom("gaus",10000);
   for (int i=27;i<38;i++) h.SetBinContent(i,666); h.Fit("gaus");
   int rng[4]={1,26,38,100}; TArrayI range(4,rng);
   FitInRange(&h,"gaus","",range); */

Int_t FitInRange(TH1* histo, const char* formula, Option_t* goption = "", TArrayI range) {
  if (range.GetSize()%2 == 1) {
    cout << "Error! The range array should have an even number of elements\n";
    return 0; }
  TH1* dummy = (TH1*)histo->Clone("dummy");
  if (range.GetSize()>3)
    for (int i=1; i<range.GetSize()/2; i++)
      for (int j=range[i*2-1]+1; j<range[i*2]; j++)
	dummy->SetBinContent(j,0);
  int fo = dummy->Fit(formula,"0",goption,dummy->GetBinLowEdge(range[0]),
		      dummy->GetBinLowEdge(range[range.GetSize()-1]+1));
  TF1 *ff = dummy->GetListOfFunctions()->Last()->Clone();
  // We fit the dummy histo without drawing. So now make the func drawable:
  ff->SetBit(TF1::kNotDraw,0);
  histo->GetListOfFunctions()->Add(ff);
  dummy->Delete();
  histo->Draw(goption);
  return fo;
}
