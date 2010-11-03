// A function to convert a 1D or 2D histogram into a variable-size bin
//  histogram of desired binning.

// Arguments are similar to the TH1 constructor with variable-size bins.
//     nbinsx  : number of bins
//     xbins   : array of low-edges for each bin
//               This is an array of size nbinsx+1
//     axisname: the axis to be rebinned, it should be "x" or "y"

// While initially the code was written to do everything manually, now it
//   internally calls the TH1::Rebin() method for 1D histograms. Still it
//   is a useful wrapper even for 1D histograms, since it can handle cases
//   with binedges not aligning properly.

// First implementation: Nov 1-3, 2010, veo.

TH1* VariableSizeRebin(TH1* inhisto, unsigned int nbinsx,
		       double *xbins, TString axisname="x",
		       TString newhistoname="newhist")
{
  if ( nbinsx == 0 ) {
    cout << "Error! nbinsx must be non-zero." << endl; return 0; }

  if ( inhisto == 0 ) {
    cout << "Error! Input histogram pointer is null." << endl; return 0; }

  if ( axisname == "y"  &&  !inhisto->InheritsFrom("TH2") ) {
    cout << "No y-axis defined for " << inhisto->GetName() << endl; return 0; }

  if ( newhistoname == "" ) {
    cout << "Error! Output histogram name is null."<< endl; return 0; }

  double *edgeArr = new double[nbinsx+2]; // an extra bin for safety

  TAxis *axis = (axisname=="y" ? inhisto->GetYaxis() : inhisto->GetXaxis());

  unsigned int nbins = 0; // number of bins for the new histogram
  unsigned int j = 0; // dummy bin index (to be used as a pointer)

  for ( unsigned int i=0; i<=axis->GetNbins()+1; ++i ) {

    if ( j > nbinsx ) break;

    double ble = axis->GetBinLowEdge(i);

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
    double xmax = axis->GetBinLowEdge(axis->GetNbins()+1);
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

  //for ( unsigned int i=0; i<=nbins; i++ )
  //  cout << "For bin " << i+1 << "\tlowedge= " << edgeArr[i] << endl;

  // Now generate the new histogram
  TH1 *newhist = 0;

  if ( !inhisto->InheritsFrom("TH2") )
    newhist = inhisto->Rebin(nbins,newhistoname.Data(),edgeArr);

  else {

    // Copy the perpendicular axis as it is.
    TAxis *axisp = (axisname=="y" ? inhisto->GetXaxis() : inhisto->GetYaxis());
    unsigned int nbinsp = axisp->GetNbins();
    double *edgeArrp = new double[nbinsp+1];
    for ( unsigned int i=1; i<=nbinsp+1; ++i )
      edgeArrp[i] = axisp->GetBinLowEdge(i);

    if ( axisname == "y" ) {

      if ( axisp->IsVariableBinSize() )
	newhist = new TH2D(newhistoname, inhisto->GetTitle(),
			   nbinsp, edgeArrp, nbins, edgeArr);
      else
	newhist = new TH2D(newhistoname, inhisto->GetTitle(),
			   nbinsp, edgeArrp[0], edgeArrp[nbinsp+1],
			   nbins, edgeArr);

      if ( axisp->GetLabels() )
	for ( unsigned int i=1; i<=nbinsp; ++i )
	newhist->GetXaxis()->SetBinLabel(i, axisp->GetBinLabel(i));

    }
    else
      // ToDo: Have not yet implemented the above nice stuff for axisname=="x"
      newhist = new TH2D(newhistoname, inhisto->GetTitle(),
			 nbins, edgeArr, nbinsp, edgeArrp);

    newhist->GetYaxis()->SetTitle(inhisto->GetYaxis()->GetTitle());
    newhist->GetXaxis()->SetTitle(inhisto->GetXaxis()->GetTitle());
    bool sw2 = ( inhisto->GetSumw2N() != 0 );

    // Fill the new histogram from the input histogram
    j=0; // reset the dummy bin index
    for ( unsigned int i=0; i<=axis->GetNbins()+1; ++i ) {

      double ble = axis->GetBinLowEdge(i);
      if ( edgeArr[j] == ble ) j++; 

      for ( unsigned int k=0; k<=nbinsp+1; ++k ) {

	int newbin(0), oldbin(0);
	// Equivalent 1D bin number = binx + (fXaxis.GetNbins()+2)*biny
	if ( axisname == "y" ) {
	  newbin = k+j*(nbinsp+2); oldbin = k+i*(nbinsp+2); }
	else {
	  newbin = j+k*(nbins+2); oldbin = i+k*(axis->GetNbins()+2); }

	newhist->SetBinContent( newbin, newhist->GetBinContent(newbin)
				+ inhisto->GetBinContent(oldbin) );
	if ( sw2 )
	  newhist->SetBinError( newbin,
			        sqrt(pow(newhist->GetBinError(newbin),2) +
				     pow(inhisto->GetBinError(oldbin),2)) );
      }
    }

    newhist->SetEntries(inhisto->GetEntries());

  }

  //newhist->Draw();
  delete [] edgeArr;
  return newhist;
}
