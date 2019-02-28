#ifndef INCLUDED_HIST_TOOLS
#define INCLUDED_HIST_TOOLS

# include "TH1D.h"

void updateOverUnderFlow(TH1D *& hist, double xmax, double xmin);
void blindAfter(TH1D * &hist, double xmax);
void zeroNegatives(TH1D *& hist);

#endif
