// A macro to take 2 histogramd and operate on them bin-by-bin.
// The cmd string can be anything accepted by ROOT CINT as valid.
//   It should be a line which gets op1 and op2 and makes a result from them.

// Examples:
// - Subtract a histo from another:
//   HistogramOperator(hist1,hist2,"result = op1 - op2")->Draw()
// - Take the maximum of each bin:
//   HistogramOperator(hist1,hist2,"result = TMath::Max(op1,op2)")->Draw()

TH1F* HistogramOperator(TH1* h1, TH1* h2, const char* cmd) {

 if ( !h1 || !h2 ) {
   cout << "Null histogram pointer(s)" << endl; return 0; }

 int nbins;
 if ( h1->GetNbinsX() != (nbins = h2->GetNbinsX() ) ) {
   cout << "Different sized histograms - cannot operate" << endl;
   return 0; }

 TH1F *out = h1->Clone();

 for (int i=0; i<=nbins; ++i) {
   double op1 = h1->GetBinContent(i);
   double op2 = h2->GetBinContent(i);
   double result = 0;
   if (cmd) gROOT->ProcessLineFast(cmd);
   out->SetBinContent(i, result);
 }

 return out;
}
