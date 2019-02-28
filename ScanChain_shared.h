// ROOT
#include "TChain.h"
#include "ConfigParser.h"
#include "ConfigHelper.h"



int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);
