#ifndef DEFINE_DATASETS
#define DEFINE_DATASETS

# include "TChain.h"
# include "TString.h"
# include "ConfigHelper.h"
# include <unordered_map>
# include <string>
# include <vector>

std::unordered_map <std::string,std::vector<std::string>> readFromTextFile(std::string fileName);

void addToChain(std::unordered_map<std::string, std::vector<std::string>> datasets,TChain *ch, TString set);

void addToChain(TChain *ch, TString set, bool hadoop, bool skimmed);

TChain * getTChain(TString data_set,std::string file_name); 
#endif
