#include "JetResolution.h"

#include <fstream>
#include <algorithm> 
#include <iostream>

#include "TObjString.h"
#include "Math/VectorUtil.h"

using namespace std;

JetResolution::JetResolution() {
  // seed 42 for reproducibility
  rndm = new TRandom3(42);
}

JetResolution::~JetResolution() { 
  if (sigma_formula) delete sigma_formula; 
  if (rndm) delete rndm;
}

void JetResolution::resetSeed(unsigned long long seed) {
    // If making multiple loops over jets,
    // want smearing to be the same for the same jet,
    // so reset the TRandom seed before each jet loop
  rndm->SetSeed(seed);
}


void JetResolution::loadResolutionFile(TString file) {

  ifstream input(file.Data());
  TString reader;

  reader.ReadLine(input);
  TObjArray* info = ((TString) reader(1,reader.Length()-2)).Tokenize(" ");

  Int_t nvars = ((TObjString*) info->At(0))->GetString().Atoi();
  for (Int_t i=1; i<nvars+1; i++) {
    sigma_varnames.push_back(((TObjString*) info->At(i))->GetString());
  }
  Int_t nparams = ((TObjString*) info->At(nvars+1))->GetString().Atoi();
  for (Int_t i=nvars+2; i<nvars+nparams+2; i++) {
    
    sigma_paramnames.push_back(((TObjString*) info->At(i))->GetString());
  }
  TString formula_text = ((TObjString*) info->At(nvars+nparams+2))->GetString();
  sigma_coefficients = 0;
  while (formula_text.Contains(TString::Format("[%d]", sigma_coefficients))) sigma_coefficients++;

  sigma_formula = new TFormula("sigma_formula", formula_text.Data());  
  
  while (!input.eof()) {
    reader.ReadLine(input);
    if (reader.Length() < 10) continue;
    info = reader.Tokenize(" ");
    vector<pair<Double_t, Double_t>> bins;
    for (Int_t i=0; i<nvars; i++) {
      bins.push_back(make_pair(((TObjString*) info->At(2*i))->GetString().Atof(), ((TObjString*) info->At(2*i+1))->GetString().Atof()));
    }
    vector<pair<Double_t, Double_t>> ranges;
    for (Int_t i=0; i<nparams; i++) {
      ranges.push_back(make_pair(((TObjString*) info->At(2*nvars+1+2*i))->GetString().Atof(), ((TObjString*) info->At(2*nvars+1+2*i+1))->GetString().Atof()));
    }
    vector<Double_t> coefficients;
    for (Int_t i=0; i<sigma_coefficients; i++) {
      coefficients.push_back(((TObjString*) info->At(2*(nvars+nparams)+1+i))->GetString().Atof());
    }
    Entry newdata;
    newdata.bins = bins;
    newdata.ranges = ranges;
    newdata.coefficients = coefficients;
    sigma_bins.push_back(newdata);
  }

  sigma_variables.resize(sigma_varnames.size());
  sigma_parameters.resize(sigma_paramnames.size());
  input.close();
}

void JetResolution::loadScaleFactorFile(TString file) {
  ifstream input(file.Data());
  TString reader;

  reader.ReadLine(input);
  TObjArray* info = ((TString) reader(1,reader.Length()-2)).Tokenize(" ");

  Int_t nvars = ((TObjString*) info->At(0))->GetString().Atoi();
  for (Int_t i=1; i<nvars+1; i++) {
    sf_varnames.push_back(((TObjString*) info->At(i))->GetString());
  }

  while (!input.eof()) {
    reader.ReadLine(input);
    if (reader.Length() < 10) continue;
    info = reader.Tokenize(" ");
    vector<pair<Double_t, Double_t>> bins;
    for (Int_t i=0; i<nvars; i++) {
      bins.push_back(make_pair(((TObjString*) info->At(2*i))->GetString().Atof(), ((TObjString*) info->At(2*i+1))->GetString().Atof()));
    }

    Double_t val = ((TObjString*) info->At(2*nvars+1))->GetString().Atof();
    Double_t errminus = ((TObjString*) info->At(2*nvars+2))->GetString().Atof();
    Double_t errplus = ((TObjString*) info->At(2*nvars+3))->GetString().Atof();

    Entry newdata;
    newdata.bins = bins;
    newdata.val = val;
    newdata.errminus = errminus;
    newdata.errplus = errplus;
    sf_bins.push_back(newdata);
  }

  sf_variables.resize(sf_varnames.size());
  input.close();
}

void JetResolution::loadVariable(TString name, Double_t val) {

  auto it_sigma_varnames = find(sigma_varnames.begin(), sigma_varnames.end(), name);
  if (it_sigma_varnames != sigma_varnames.end()) sigma_variables[distance(sigma_varnames.begin(), it_sigma_varnames)] = val;

  auto it_sigma_paramnames = find(sigma_paramnames.begin(), sigma_paramnames.end(), name);
  if (it_sigma_paramnames != sigma_paramnames.end()) sigma_parameters[distance(sigma_paramnames.begin(), it_sigma_paramnames)] = val;

  auto it_sf_varnames = find(sf_varnames.begin(), sf_varnames.end(), name);
  if (it_sf_varnames != sf_varnames.end()) sf_variables[distance(sf_varnames.begin(), it_sf_varnames)] = val;

}

Double_t JetResolution::getResolution() {

  for (auto&& bin : sigma_bins) {
    
    Bool_t isThisBin = true;
    for (Int_t i=0; i<static_cast<Int_t>(sigma_variables.size()); i++) {
      if (sigma_variables[i] < bin.bins[i].first || sigma_variables[i] > bin.bins[i].second) { 
	isThisBin = false;
	break;
      }
    }
    if (!isThisBin) continue;
    
    for (Int_t i=0; i<static_cast<Int_t>(sigma_parameters.size()); i++) {
      if (sigma_parameters[i] < bin.ranges[i].first) sigma_parameters[i] = bin.ranges[i].first;
      else if (sigma_parameters[i] > bin.ranges[i].second) sigma_parameters[i] = bin.ranges[i].second;
    }

    return sigma_formula->EvalPar(sigma_parameters.data(), bin.coefficients.data());
    
  }
    
  return -1.;

}

Double_t JetResolution::getScaleFactor(Int_t syst, Int_t year) 
{
  Double_t sf;
  Double_t error;
  if(year == 2018)
  {
      sf = 1.243;
      error = 0.041;
  }
  else if(year == 2017)
  {
      sf = 1.092;
      error = 0.212;
  }
  else if(year == 2018)
  {
      sf = 1;
      error = 0.2;
  }
  if (syst == 0) 
      return sf;
  else if (syst == -1) 
      return sf - error;
  else if (syst == 1) 
      return sf + error;

  return -1.;

}


vector<Double_t> JetResolution::smear(Int_t syst, Int_t year) {
  
  vector<Double_t> retval;
  
  Double_t sf = getScaleFactor(syst,year); if (sf < 1) return sigma_parameters;
  Double_t res = getResolution(); if(res<0) return sigma_parameters;
    
  Double_t Rcone = 0.4;
  for (size_t ipar=0; ipar < sigma_parameters.size() ; ipar++) {
    auto par = sigma_parameters.at(ipar);
    Bool_t isMatched = false;
    
    retval.push_back((1.+rndm->Gaus(0.,res)*sqrt(max(sf*sf-1.,0.))));
    }
  
  
  return retval;

}
 
