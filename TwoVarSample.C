// A class to keep track of means, variances and correlations of two
//  variables. Example use:
// TwoVarSample v;  // One of the Anscombe's quartet
// v.addData(8, 6.58); v.addData(8, 5.76); v.addData(8, 7.71);
// v.addData(8, 8.84); v.addData(8, 8.47); v.addData(8, 7.04);
// v.addData(8, 5.56); v.addData(8, 7.91); v.addData(8, 6.89);
// v.addData(8, 5.25); v.addData(19, 12.5);
// cout << v.getCorrCoef() << endl;

class TwoVarSample {
private:
  double xy, x2, y2, x, y;
  int n;
public:
  TwoVarSample() { n = 0; xy = x2 = y2 = x = y = 0; }
  void addData( double xi, double yi ) {
    n++; xy += xi*yi; x2 += xi*xi; y2 += yi*yi; x += xi; y += yi; }
  int getN() { return n; }
  double getMeanX() { return x/n; }
  double getMeanY() { return y/n; }
  double getVarX()  { return x2/n - getMeanX()*getMeanX(); }
  double getVarY()  { return y2/n - getMeanY()*getMeanY(); }
  double getSDevX() { return sqrt( getVarX() ); }
  double getSDevY() { return sqrt( getVarY() ); }
  double getCov()   { return xy/n - x*y/n/n; }
  double getCorrCoef() { return getCov()/(getSDevX()*getSDevY()); }
};
