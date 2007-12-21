// A function which can return a list of possible user defined functions
TF1* UserFunc(TString function, TString name="user",
	      double xmin=0, double xmax=1, double *param=0) {
  function.ToLower();
  TF1 *func=0;
  // Gaussian defined by its area, mean and sigma
  if ( function == "gaussian" || function == "normal" ) {
    func = new TF1(name, "[0]/2.50662827463100024/[2]*TMath::Gaus(x,[1],[2])");
    func->SetParNames("A_{sig}", "#mu_{sig}", "#sigma_{sig}");
    func->SetParameters(1,0,1); }
  // Exponential which accepts a shift, x_0
  else if ( function == "exp" || function == "exponential" ) {
    func = new TF1(name, "[0]*TMath::Exp(-1/[1]*(x-[2]))");
    func->SetParNames("c_{bg}", "#tau_{bg}", "offset_{bg}");
    func->SetParameters(1,1,0); }
  // Breit-Wigner defined by its area, MPV and HWHM
  else if ( function == "bw" || function == "breit-wigner" ||
	    function == "cauchy" || function == "lorentz" ) {
    func = new TF1(name, "[0]*[2]/TMath::Pi()/((x-[1])*(x-[1])+[2]*[2])");
    func->SetParNames("A_{sig}","mpv_{sig}","#gamma_{sig}");
    func->SetParameters(1,0,1); }
  // Xtal Ball
  else if ( function == "cb" || function == "crystal ball" ) {
    func = new TF1(name,XtalBall,xmin,xmax,5);
    func->SetParNames("N","mpv","#sigma","#alpha","n");
    func->SetParameters(1,0,0.5,1,10); }
  // Double Gaussian with the mpvs the same
  else if ( function == "doublegaus" ) {
    func = new TF1(name, "[0]*([1]*TMath::Gaus(x,[2],[3])+(1-[1])*TMath::Gaus(x,[2],[3]*[4]))");
    func->SetParNames("norm","f_{core}","mpv","#sigma_{core}","#sigma_{wide}/#sigma_{core}");
    func->SetParLimits(1,0,1);
    func->SetParameters(1,0.8,0,1,10); }
  func->SetRange(xmin,xmax);
  if (param!=0) func->SetParameters(param);
  return func;
}

Double_t CrystalBall(Double_t *x, Double_t *par)
{
  Double_t xx = x[0];
  Double_t N = par[0], m = par[1], sig = par[2], alp = par[3], n = par[4];
  if (sig==0) return 0;
  if ( xx > m-alp*sig )
    return N * exp(-(xx-m)*(xx-m)/2/sig/sig);
  if (alp==0) return 0;
  if (m/sig-xx/sig+n/alp-alp==0) return 0;
  return N * exp(-alp*alp/2) * pow(n/alp/(m/sig-xx/sig+n/alp-alp),n);
}

// reverse CrystalBall : flipped around x=mpv
Double_t XtalBall(Double_t *x, Double_t *par) {
  Double_t xprime = 2*par[1]-x[0]; return CrystalBall(&xprime, par); }
