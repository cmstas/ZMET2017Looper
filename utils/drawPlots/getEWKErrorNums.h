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
std::vector<TString> getFileLocation(std::string basePath,TString sample_name);
std::pair<double, std::vector<double>> getEWKNumsForSample(TString sample_name,bool allSR = false,std::string basePath="");
std::vector<double> getPercentStatErrorsForNoEWKSub(TString sample_name,bool allSR = false,std::string basePath="");
void getEWKErrorNums();
#endif
