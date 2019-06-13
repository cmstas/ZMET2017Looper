#ifndef DRAW_PLOTS_H
#define DRAW_PLOTS_H

//C++
# include <iostream>
# include <stdexcept>
# include <vector>
# include <algorithm>

//ROOT
# include "TEnv.h"
# include "TChain.h"
# include "TLatex.h"
# include "TLegend.h"
# include "TCanvas.h"
# include "TColor.h"
# include "THStack.h"
# include "TGaxis.h"
# include "TCut.h"
# include "TH1D.h"

//LaTeX and TDR tdrstyles
# include "CTable.h"
# include "tdrstyle_SUSY.h"

//other headers
# include "computeErrors.h"
# include "ConfigParser.h"
# include "ConfigHelper.h"
# include "HistTools.h"

double errMult(double A, double B, double errA, double errB, double C);
std::vector<TString> split_histogram_names(TString histNameString);
bool histOrdering(TH1D *h1, TH1D *h2);
void assignColor(std::vector<TH1D*> hists);
TH1D *combine_histograms(TFile *hist_file, std::vector<TString> hist_names, int count, TString plot_name,TString SR="");
float err_binomial(float A, float B, float errA, float errB);
void drawLatexFromTString(TString text, double x_low, double y_low);
bool TH1DIntegralSort(TH1D *hist_1, TH1D *hist_2);
bool LabeledHistSort(std::pair<TH1D *,TString> hist_1, std::pair<TH1D*, TString> hist_2);
void drawCMSLatex(double luminosity, TString cms_label = "");
void drawSRText(TString SR, double high_y, double low_x);
TString drawArbitraryNumberWithResidual(ConfigParser *conf,TString SR="");
TString drawArbitraryNumber(ConfigParser *conf);
TString drawSingleTH1(ConfigParser *conf);
TString drawCutDebug(ConfigParser *conf);
TString drawCutDebug(TString sample_name, TString sample_loc, TString save_dir);
TString drawWeightDebug(TString sample_name, TString sample_loc, TString save_dir, TString hist_name);
TString drawDebugPlots(ConfigParser *conf);
TString drawSingleTH2(ConfigParser *conf);
void drawPlots(TString config_file, TString SR="",bool draw_debugs = true);
#endif
