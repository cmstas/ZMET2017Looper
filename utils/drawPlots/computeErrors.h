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

std::vector<double> getMetTemplatesError(const std::vector<double> &stat_err, const std::vector<double> &bin_count, double normalization, int norm_bin, const std::vector<std::pair<double, double>> &bin_edge, TString SR,bool allSR=false,std::string EWK_base_path="");

//single overall Rsfof
std::pair<std::vector<double>,std::vector<double>> getFSError(const std::vector<double> &bin_count, double RSFOFxKappa, double Kappa_stat_error, TString SR);

//year by year Rsfof
std::pair<std::vector<double>,std::vector<double>> getFSError(const std::vector<double> &bin_count,const std::vector <double> &bin_count_2016, const std::vector<double> &bin_count_2017, const std::vector<double> &bin_count_2018, double Kappa, double Kappa_stat_error,TString SR);

//event by event Rsfof - factorization method
std::pair<std::vector<double>,std::vector<double>> getFSError(const std::vector<double> &bin_count, const std::vector<double> &norm_up, const std::vector<double> &norm_down, const std::vector<double> &pt_up, const std::vector<double> &pt_down, const std::vector<double> &eta_up, const std::vector<double> &eta_down,double Kappa, double Kappa_stat_error, TString SR);

std::vector<double> getRareSamplesError(std::unordered_map<std::string,std::vector<double>>& all_errors, const std::vector<double> &bin_count, float scale, float scale_stat_unc, double scale_syst_unc, TString SR);

void printErrors(const std::vector<double> &temp_err, const std::vector<double> &rare_err, const std::pair<std::vector<double>, std::vector<double>> &fs_err, const std::vector<double> &bin_low);

TGraphAsymmErrors* getErrorTGraph(const std::vector<double> &temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/,bool ratioError=false, const std::vector<double>& tau21_error_up={},const std::vector<double> &tau21_error_down={});

void printCounts(const std::vector<double> &temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/);

void printLatexCounts(const std::vector<double> temp_count, const std::vector<double> &temp_err, const std::vector<double> &rare_count, const std::vector<double> &rare_err, const std::vector<double> &fs_count, const std::pair<std::vector<double>,std::vector<double>> &fs_err, const std::vector<std::pair<double,double>> &bin_low, const std::vector<double> &data_count, double RSFOF /*Really just the scale factor*/);

std::pair<std::vector<double>,std::vector<double>> getTau21Error(const std::vector<double> count_central_2016,std::vector<double> count_central_2017,std::vector<double> count_central_2018,const std::vector<double> count_tau21_up_2016,std::vector<double> count_tau21_up_2017,const std::vector<double> count_tau21_up_2018,const std::vector<double> count_tau21_down_2016,const std::vector<double> count_tau21_down_2017,const std::vector<double> count_tau21_down_2018);
void computeErrors();
#endif
