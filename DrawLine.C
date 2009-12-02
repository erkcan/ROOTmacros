// Draw a line with the equation y = slope * x + offset
// on an already-drawn TGraph2D or on a histogram. The
// function returns a TObject*, which inherits from either
// a TLine* or a TPolyLine3D* depending on the gPad.
// If you cannot obtain what you desire (for instance if you
// get too short lines, or weird line labels), please try
// gPad->Update() before calling DrawLine().

// Known issues: There are no 3d text objects in ROOT. Because
// of this, line label is located correctly only for TH1 plots
// and contour plots of TGraph2D.

TObject* DrawLine(float slope, float offset, TString label="") {

  TObject* ll=0;
  // loop through all the things drawn on the currently
  // active TPad, until you find a TGraph2D or TH2*.
  const TList *padlist = gPad->GetListOfPrimitives();
  for (int pp=0; pp<padlist->GetEntries(); ++pp) {
    TObject *myobj = padlist->At(pp);
    if ( myobj != 0 && myobj->InheritsFrom("TGraph2D") )
      myobj = ((TGraph2D*)myobj)->GetHistogram();
    if ( myobj != 0 && myobj->InheritsFrom("TH1") ) {
      float xmin=((TH1*)myobj)->GetXaxis()->GetXmin();
      float xmax=((TH1*)myobj)->GetXaxis()->GetXmax();
      float ymin=((TH1*)myobj)->GetYaxis()->GetXmin();
      float ymax=((TH1*)myobj)->GetYaxis()->GetXmax();
      // For TH1 type histos, GetYaxis()->GetX???() methods
      // do not work properly. So use gPad->GetUy???().
      if ( ! myobj->InheritsFrom("TH2") ) {
	ymin=gPad->GetUymin(); ymax=gPad->GetUymax(); }
      Float_t x[2]={xmin,xmax};
      if ( xmin*slope+offset > ymax || xmin*slope+offset < ymin )
	x[0] = ((slope>0?ymin:ymax)-offset)/slope;
      if ( xmax*slope+offset > ymax || xmax*slope+offset < ymin )
	x[1] = ((slope<0?ymin:ymax)-offset)/slope;
      if ( x[0]<xmin || x[1]>xmax ) break;
      Float_t y[2]={x[0]*slope+offset,slope*x[1]+offset};
      Float_t z[2]={0,0};
      if ( myobj->InheritsFrom("TH2") ) ll = new TPolyLine3D(2,x,y,z);
      else ll = new TLine(x[0],y[0],x[1],y[1]);
      ll->Draw();
      // Print a "label" in the middle of the line
      if ( label != "" ) {
	TLatex *tex = new TLatex();
	tex->SetTextSize(0.03);
	// need the following, since slope!=plot slope
	double ta = (gPad->YtoPixel(y[1])-gPad->YtoPixel(y[0])) *-1.
	  / (gPad->XtoPixel(x[1])-gPad->XtoPixel(x[0]));
	tex->SetTextAngle(TMath::ATan(ta)*180*TMath::InvPi());
	// for a contour plot, need world coor. -> user coor.
	TView* view=gPad->GetView();
	if ( view ) {
	  Float_t  wc[3]={ (x[0]+x[1])/2, (y[0]+y[1])/2, 1 };
	  Float_t ndc[3]={ 0,0,0 };
	  view->WCtoNDC(wc,ndc);
	  tex->DrawText( ndc[0], ndc[1], label ); }
	else tex->DrawText( (x[0]+x[1])/2, (y[0]+y[1])/2, label ); }
      break;
    }
  }
  return ll;

}
