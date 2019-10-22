#ifndef ZMET_LOOPER
#define ZMET_LOOPER

# include "TString.h"
# include "ConfigParser.h"
# include "ScanChain.h"
# include "DefineDatasets.h"
# include "makePtReweightHisto.h"

void runScanChain(ConfigParser *conf);
void doAll(TString config_name, TString config_file = "configs/run_modes.conf");

#endif
