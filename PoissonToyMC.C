// An example toy MC generator. It generates floating point
// numbers according to a Poisson distribution of chosen mean,
// then calculates the chi2 probability for each generated
// number and tries to see if a given CL is reached or not.
// It can be thought of a test of how good the random number
// generation in ROOT is.

float PoissonToyMC(float mean, float CL=0.95, long trials=1e4)
{
  TF1 balik("balik","TMath::Poisson(x,[0])",0,100);
  balik.SetParameter(0,mean);
  float histrng = TMath::Ceil(mean*2.5);
  TH1F *hal = new TH1F("hal","ToyPoisson",histrng,0,histrng);
  long pass=0;
  for (long k=0; k<trials; ++k) {
    //int uydur = (int)(TMath::Floor(0.5+(balik.GetRandom())));
    float uydur = balik.GetRandom();
    hal->Fill(uydur);
    if ( TMath::Prob(pow(uydur-mean,2)/mean,1) < CL ) pass++;
  }
  hal->Draw();
  return pass/float(trials);
}
