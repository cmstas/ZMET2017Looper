#ifndef INCLUDED_HIST_TOOLS
#define INCLUDED_HIST_TOOLS
# include "TH1D.h"
# include "TH2D.h"
# include <vector>
# include <string>
# include <unordered_map>
# include <cstring>


void updateOverUnderflow(TH1D *& hist, double xmax, double xmin = -1000000);
void blindAfter(TH1D * &hist, double xmax);
void zeroNegatives(TH1D *& hist);
#endif
