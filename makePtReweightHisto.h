#ifndef PT_REWEIGHT_HISTO
#define PT_REWEIGHT_HISTO

# include "TH1D.h"
# include "ConfigParser.h"
# include "ConfigHelper.h"
# include "HistTools.h"
# include "TFile.h"

using namespace std;

void makePtReweightHisto(ConfigParser *conf);
void makePtReweightHisto_allSR(ConfigParser *conf);
#endif
