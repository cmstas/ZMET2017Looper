#ifndef INCLUDED_HIST_TOOLS
#define INCLUDED_HIST_TOOLS
# include "TH1D.h"
# include <vector>
# include <string>
# include <unordered_map>
# include <cstring>


void updateOverUnderFlow(TH1D *& hist, double xmax, double xmin);
void blindAfter(TH1D * &hist, double xmax);
void zeroNegatives(TH1D *& hist);
void fill1DHistograms(std::string name, float xval, double weight, std::unordered_map<std::string,TH1*> &allHistos, const char *title,int nbins,double xmin, double xmax,TDirectory *rootdir);
void fill1DHistograms(std::string name, float xval, double weight, std::unordered_map<std::string, TH1*> &allHistos, const char *title, int nbins, const double *xbins,TDirectory *rootdir);
void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, double xmin, double xmax, int nbinsy, double ymin, double ymax, TDirectory *rootdir);
void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, const double *xbins, int nbinsy, double ymin, double ymax, TDirectory *rootdir);
void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, const double *xbins, int nbinsy, const double *ybins, TDirectory *rootdir);
void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, double xmin, double xmax, int nbinsy, const double *ybins, TDirectory *rootdir);



#endif
