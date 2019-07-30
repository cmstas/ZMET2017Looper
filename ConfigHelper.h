# include "ConfigParser.h"
#include "TString.h"

#ifndef CONFIG_HELPER
#define CONFIG_HELPER

extern TString PLOT_OUTPUT_LOCATION;
extern TString HIST_OUTPUT_LOCATION;

TString parseConfDir(TString conf_path);
TString getOutputDir(ConfigParser *conf, TString type,int year = -1);
TString getDefaultHistDir(ConfigParser *conf);
TString parseLatex(TString opt);
std::vector<double> parseVector(TString opt);
std::vector<int> iparseVector(TString opt);
std::vector<TString> sParseVector(TString opt);

#endif

