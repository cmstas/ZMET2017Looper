#ifndef INCLUDED_COMPUTE_ERROS
#define INCLUDED_COMPUTE_ERROS

#include <vector>
#include <utility>
#include <sstream>
#include "TString.h"
#include "RooHistError.h"
#include "TGraphAsymmErrors.h"
#include "getEWKErrorNums.h"

double err_divide(double A, double B, double errA, double errB);

void printTemplatesDebug(const std::vector<double> &prediction, const std::vector<double> &prediction_err, const std::vector<double> &stat_err, const std::vector<double> &closure_err, const std::vector<double> &norm_err, const std::vector<double> &ewk_err, const std::vector<std::pair<double, double>> &bin_edge);

std::vector<double> getMetTemplatesError(const std::vector<double> &stat_err, const std::vector<double> &bin_count, double normalization, int norm_bin, const std::vector<std::pair<double, double>> &bin_edge, TString SR,bool allSR=false);

std::pair<std::vector<double>,std::vector<double>> getFSError(const std::vector<double> &bin_count, double RSFOFxKappa, TString SR);

std::vector<double> getRareSamplesError(const std::vector<double> &stat_err, const std::vector<double> &bin_count, double scale, double scale_unc, TString SR);

void printErrors(const std::vector<double> &temp_err, const std::vector<double> &rare_err, const std::pair<std::vector<double>, std::vector<double>> &fs_err, const std::vector<double> &bin_low);

TGraphAsymmErrors* getErrorTGraph(const std::vector<double> &temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/);

void printCounts(const std::vector<double> &temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/);

void printLatexCounts(const std::vector<double> temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/);

void computeErrors();
#endif
