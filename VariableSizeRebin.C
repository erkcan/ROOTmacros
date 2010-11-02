// A function to convert a histogram into a variable-size bin histogram of
//  desired binning.

// Arguments are similar to the TH1 constructor with variable-size bins.
//     nbins  : number of bins
//     xbins  : array of low-edges for each bin
//              This is an array of size nbins+1

// While initially the code was written to do everything manually, now it
//   internally calls the TH1::Rebin() method. Still it is a useful wrapper,
//   since it can handle cases with binedges not aligning properly.

TH1D* VariableSizeRebin(TH1* inhisto, unsigned int nbinsx, double *xbins)
{
  if ( nbinsx == 0 ) {
    cout << "Error! nbinsx must be non-zero." << endl; return 0; }

  double *edgeArr = new double[nbinsx+2]; // an extra bin for safety

  unsigned int nbins = 0; // number of bins for the new histogram
  unsigned int j = 0; // dummy bin index (to be used as a pointer)

  for ( unsigned int i=0; i<=inhisto->GetNbinsX()+1; ++i ) {

    if ( j > nbinsx ) break;

    double ble = inhisto->GetBinLowEdge(i);

    if ( xbins[j] > ble ) continue;

    edgeArr[nbins] = ble; j++; nbins++;

    if ( xbins[j-1] < ble ) {
      cout << "Warning! Bin edge at " << xbins[j-1] << " does not align with"
	   << " input histo. Realigning at " << ble << ".\n";
      // check if the upcoming bin edges become obsolete after realigning.
      while ( j<=nbinsx && xbins[j] <= ble ) j++;
    }

  }

  // if we finished the loop normally, ie. not 'break'ing out, it must be
  // that the input histogram xrange is shorter than what the new binning
  // tried to get. So handle that.
  if ( j <= nbinsx ) {
    double xmax = inhisto->GetBinLowEdge(inhisto->GetNbinsX()+1);
    if ( xmax>edgeArr[nbins-1] ) {
      edgeArr[nbins]=xmax;
      cout << "Warning! Input histo reached max value of its x-range. "
	   << "Last bin to be closed at " << edgeArr[nbins] << "." << endl;
      nbins++; }
  }

  // we go out of the loop when index j overshoots. So our nbins is
  // always one more than actual number of bins. Fix that.
  nbins--;

  if ( nbinsx != nbins )
    cout << "Warning! nbinsx set to " << nbins
	 << " instead of " << nbinsx << "." << endl;

  for ( unsigned int i=0; i<=nbins; i++ )
    cout << "For bin " << i << "\tlowedge= " << edgeArr[i] << endl;

  // Now book the new histogram
  /*
  TH1D *newhist = new TH1D("newhist",inhisto->GetTitle(),nbins,edgeArr);
  newhist->GetYaxis()->SetTitle(inhisto->GetYaxis()->GetTitle());
  newhist->GetXaxis()->SetTitle(inhisto->GetXaxis()->GetTitle());
  bool sw2 = ( inhisto->GetSumw2N() != 0 );

  // Fill the new histogram from the input histogram
  j=0; // reset the dummy bin index
  for ( unsigned int i=0; i<=inhisto->GetNbinsX()+1; ++i ) {

    double ble = inhisto->GetBinLowEdge(i);
    if ( edgeArr[j] == ble ) j++; 
      
    newhist->SetBinContent( j, newhist->GetBinContent(j)
			    + inhisto->GetBinContent(i) );
    if ( sw2 ) newhist->SetBinError(j, sqrt(pow(newhist->GetBinError(j),2) +
					    pow(inhisto->GetBinError(i),2)));}

  newhist->SetEntries(inhisto->GetEntries());
  */
  TH1D *newhist = inhisto->Rebin(nbins,"newhist",edgeArr);

  //newhist->Draw();
  delete [] edgeArr;
  return newhist;
}
