#ifndef EWK_ERROR_NUMS
#define EWK_ERROR_NUMS

#include "TString.h"
#include "TH1.h"
#include "TFile.h"
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>

#include "../../HistTools.h"
std::vector<TString> getFileLocation(TString sample_name);
std::pair<double, std::vector<double>> getEWKNumsForSample(TString sample_name);
std::vector<double> getPercentStatErrorsForNoEWKSub(TString sample_name);
void getEWKErrorNums();
#endif
