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
