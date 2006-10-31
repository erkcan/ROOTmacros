// Draw a line with the equation y = slope * x + offset
// on an already-drawn TGraph2D or on a TH2*.

TPolyLine3D* DrawLine(float slope, float offset) {

  TPolyLine3D* ll=0;
  // loop through all the things drawn on the currently
  // active TPad, until you find a TGraph2D or TH2*.
  const TList *padlist = gPad->GetListOfPrimitives();
  for (int pp=0; pp<padlist->GetEntries(); ++pp) {
    TObject *myobj = padlist->At(pp);
    if ( myobj != 0 && myobj->InheritsFrom("TGraph2D") )
      myobj = ((TGraph2D*)myobj)->GetHistogram();
    if ( myobj != 0 && myobj->InheritsFrom("TH2") ) {
      float xmin=((TH1*)myobj)->GetXaxis()->GetXmin();
      float xmax=((TH1*)myobj)->GetXaxis()->GetXmax();
      float ymin=((TH1*)myobj)->GetYaxis()->GetXmin();
      float ymax=((TH1*)myobj)->GetYaxis()->GetXmax();
      if ( xmin*slope+offset > ymax || xmin*slope+offset < ymin )
	xmin = ((slope>0?ymin:ymax)-offset)/slope;
      if ( xmax*slope+offset > ymax || xmax*slope+offset < ymin )
	xmax = ((slope<0?ymin:ymax)-offset)/slope;
      Float_t x[2]={xmin,xmax};
      Float_t y[2]={xmin*slope+offset,slope*xmax+offset};
      Float_t z[2]={0,0};
      ll = new TPolyLine3D(2,x,y,z);
      ll->Draw();
      break;
    }
  }
  return ll;

}
