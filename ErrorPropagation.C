Double_t CalcError(Double_t x, Double_t dx, Double_t z, Double_t dz, char op)
{
  // This is a function to do error propogation for certain operations.
  // op can be : 'B' - binomial division, '+','-','*','/' - arithmetic operations

  if (op=='b' || op=='B') {
    Double_t  y  = z - x;
    Double_t dy2 = dz*dz - dx*dx;
    return ( z ? (sqrt(y*y*dx*dx + x*x*dy2) /z/z) : 0 ); }
  else if (op=='+' || op=='-') return (Double_t)sqrt(dx*dx+dz*dz);
  else if (op=='*') return (Double_t)sqrt(dx*dx*z*z+dz*dz*x*x);
  else if (op=='/' && z!=0) return (Double_t)x/z*sqrt(dx*dx/x/x+dz*dz/z/z);
  else return 0;

}

TString BinomialDiv(Double_t dividend, Double_t divisor, Bool_t percent=0)
{
  // Returns a TString for the result of a division operation and the
  // associated binomial division error. Handy for efficiency calculations
  // in physics. The last parameter, which is false by default, can be
  // set true when the answer is desired in percent.
  Double_t perfac=1.; TString persign="";
  if (percent) { perfac=100.; persign=" %"; }
  TString out=""; out+=(dividend/divisor*perfac); out+=persign+" +- ";
  out+=perfac*CalcError(dividend, sqrt(dividend), divisor, sqrt(divisor), 'b');
  return out+persign;
}
