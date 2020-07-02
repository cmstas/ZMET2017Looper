#ifndef HPP_JETRES
#define HPP_JETRES

#include <vector>
#include <utility>
#include "TFormula.h"
#include "TString.h"
#include "TRandom3.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

struct Entry {
  
  std::vector<std::pair<Double_t,Double_t>> bins;
  std::vector<std::pair<Double_t,Double_t>> ranges;
  std::vector<Double_t> coefficients;

  Double_t val;
  Double_t errplus;
  Double_t errminus;

};

class JetResolution {

 private:
  TRandom3* rndm;

 protected:  
  std::vector<TString>  sigma_varnames;
  std::vector<TString>  sigma_paramnames;

  TFormula*             sigma_formula;
  Int_t                 sigma_coefficients;

  std::vector<Entry>    sigma_bins;

  std::vector<Double_t> sigma_variables;
  std::vector<Double_t> sigma_parameters;

  std::vector<TString>  sf_varnames;
  std::vector<Entry>    sf_bins;
  std::vector<Double_t> sf_variables;

  std::vector<Bool_t>   sigma_isMatched;
  
 public:
  JetResolution();
  ~JetResolution();

  void resetSeed(unsigned long long seed);
  void loadResolutionFile(TString file);
  void loadScaleFactorFile(TString file);
  void loadVariable(TString name, Double_t val);

  Double_t getResolution();
  Double_t getScaleFactor(Int_t syst, Int_t year);

  std::vector<Double_t> smear(Int_t syst, Int_t year);

};

#endif
