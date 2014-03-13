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
	TString rcrsv = PrintLastFit((TH1*)myobj);
	if (rcrsv!="") out=rcrsv;
      }
    }
    return out;
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
// Note: If you get an abnormal termination of minimization warning,
// try to modify the range.

Int_t FitInRange(TH1* histo, const char* formula, Option_t* goption, TArrayI range) {
  TH1* dummy = SelectBinsFromHisto(histo,range);
  if (dummy==0) return 0;
  int fo = dummy->Fit(formula,"0",goption,dummy->GetBinLowEdge(range[0]),
		      dummy->GetBinLowEdge(range[range.GetSize()-1]+1));
  CarryFuncFromHisto2Histo(dummy, histo); dummy->Delete();
  histo->Draw(goption);
  return fo;
}

Int_t FitInRange(TH1* histo, TF1 *f1, Option_t* goption, TArrayI range) {
  TH1* dummy = SelectBinsFromHisto(histo,range);
  if (dummy==0) return 0;
  int fo = dummy->Fit(f1,"0",goption,dummy->GetBinLowEdge(range[0]),
		      dummy->GetBinLowEdge(range[range.GetSize()-1]+1));
  CarryFuncFromHisto2Histo(dummy, histo); dummy->Delete();
  histo->Draw(goption);
  return fo;
}


// A function that returns a new histo from a given histo which
// contains only the selected bins.

TH1* SelectBinsFromHisto(TH1* histo, TArrayI range) {
  if (range.GetSize()%2 == 1) {
    cout << "Error! The range array should have an even number of elements\n";
    return 0; }
  TH1* dummy = (TH1*)histo->Clone("dummy");
  if (range.GetSize()>3)
    for (int i=1; i<range.GetSize()/2; i++)
      for (int j=range[i*2-1]+1; j<range[i*2]; j++)
	{ dummy->SetBinContent(j,0); dummy->SetBinError(j,0); }
  return dummy;
}


// A function to carry the last fit function from one histogram
// to another histogram.  Useful if you would like to compare the
// fits to two similar histograms.

void CarryFuncFromHisto2Histo(TH1 *from, TH1* dest) {
  TF1 *ff = (TF1*)from->GetListOfFunctions()->Last()->Clone();
  // The function we got might not be drawable, so make it:
  ff->SetBit(TF1::kNotDraw,0);
  dest->GetListOfFunctions()->Add(ff);
  return;
}
