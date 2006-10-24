// An example toy MC generator. It generates floating point
// numbers according to a Poisson distribution of chosen mean,
// then calculates the chi2 probability for each generated
// number and tries to see if a given CL is reached or not.
// It can be thought of a test of how good the random number
// generation in ROOT is.

#define _IntRand_

float PoissonToyMC(double mean, float CL=0.95, long trials=1e4)
{
  float histrng = TMath::Ceil(mean*2.5);
  TF1 balik("balik","TMath::Poisson(x,[0])",0,histrng);
  balik.SetParameter(0,mean);
  TH1F *hal = new TH1F("hal","ToyPoisson",histrng,0,histrng);
  long pass=0;
  for (long k=0; k<trials; ++k) {
#ifndef _IntRand_
    // The following works out with a floating point distro:
    float uydur = balik.GetRandom();
    if ( TMath::Prob(pow(uydur-mean,2)/mean,1) < CL ) pass++;
#else
    int uydur = gRandom->Poisson(mean);
    //int uydur = (int)(TMath::Floor(0.5+(balik.GetRandom())));
    // If balik is defined with TMath::PoissonI, use this one:
    //int uydur = (int)(TMath::Floor(balik.GetRandom()));
    // But use this one for integers (like numbers of events):
    if ( 0.5*TMath::ChisquareQuantile(CL,2*uydur+2)>=mean &&
	 0.5*TMath::ChisquareQuantile(1-CL,2*uydur)<=mean ) pass++;
#endif
    hal->Fill(uydur);
  }
  hal->Draw();
  return pass/float(trials);
}
