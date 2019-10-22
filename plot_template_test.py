from __future__ import print_function
import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os

prefix = "/home/users/bsathian/ZMet/histstemplate_comparison/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/template_comparison/"  #ORGANIZED THE OTHER WAY ROUND, ADD MCTemplates while saving!


def integrate_histograms(hist):
    scale = hist.Integral(hist.FindBin(50),hist.FindBin(99.99))
    return scale

binning = np.array([0,50,100,150,230,300,600],dtype = np.float64)
lumi = {"2016":"35.9","2017":"41.5","2018":"59.8"}
SRs = [""]
for year in ["2017"]:
    gammaFile = r.TFile(os.path.join(prefix,"reweightGamma",year,"GammaData-EWKSub.root"))
    gammaFile_unweighted = r.TFile(os.path.join(prefix,"cleanGamma",year,"GammaData-EWKSub.root"))
    ZFile = r.TFile(os.path.join(prefix,"cleanZ",year,"Data.root"))

#Create ZFile MET subtraction histogram
    TTZFile = r.TFile(os.path.join(prefix,"cleanZ",year,"TTZ.root"))
    VVVFile = r.TFile(os.path.join(prefix,"cleanZ",year,"VVV.root"))
    WZTo2L2QFile = r.TFile(os.path.join(prefix,"cleanZ",year,"WZTo2L2Q.root"))
    WZTo3LNuFile = r.TFile(os.path.join(prefix,"cleanZ",year,"WZTo3LNu.root"))
    ZZ2LFile = r.TFile(os.path.join(prefix,"cleanZ",year,"ZZ2L.root"))
    ZZ4LFile = r.TFile(os.path.join(prefix,"cleanZ",year,"ZZ4L.root"))



    gamma_hist = gammaFile.Get("type1MET").Rebin(len(binning)-1,"rebin_met_mc",binning)
    gamma_hist_unweighted = gammaFile_unweighted.Get("type1MET").Rebin(len(binning)-1,"rebin_met_mc_unweighted",binning)

    Z_hist = ZFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_met_Z_mc",binning)
    temp = ZFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_met_mumu_Z_mc",binning)
    Z_hist.Add(temp)

    TTZhist = TTZFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_met_TTZ",binning)
    TTZhist_temp = TTZFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_met_ttz_mumu",binning)
    TTZhist.Add(TTZhist_temp)
    TTZhist.Scale(1.09)

    VVVhist = VVVFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_MET_VVV",binning)
    VVVhist_temp = VVVFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_MET_VVV_mumu",binning)
    VVVhist.Add(VVVhist_temp)
    VVVhist.Scale(1)

    ZZ4Lhist = ZZ4LFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_MET_ZZ4L",binning)
    ZZ4Lhist_temp = ZZ4LFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_MET_ZZ4L_mumu",binning)
    ZZ4Lhist.Add(ZZ4Lhist_temp)
    ZZ4Lhist.Scale(1.8)

    ZZ2Lhist = ZZ2LFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_MET_ZZ2L",binning)
    ZZ2Lhist_temp = ZZ2LFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_MET_ZZ2L",binning)
    ZZ2Lhist.Add(ZZ2Lhist_temp)
    ZZ2Lhist.Scale(1.8)

    WZTo3LNuhist = WZTo3LNuFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_MET_WZTo3LNu",binning)
    WZTo3LNuhist_temp = WZTo3LNuFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_MET_WZTo3LNu_mumu",binning)
    WZTo3LNuhist.Add(WZTo3LNuhist_temp)
    WZTo3LNuhist.Scale(0.94)

    WZTo2L2Qhist = WZTo2L2QFile.Get("ee_type1MET").Rebin(len(binning)-1,"rebin_MET_WZTo2L2Q",binning)
    WZTo2L2Qhist_temp = WZTo2L2QFile.Get("mumu_type1MET").Rebin(len(binning)-1,"rebin_MET_WZTo2L2Q_mumu",binning)
    WZTo2L2Qhist.Add(WZTo2L2Qhist_temp)
    WZTo2L2Qhist.Scale(0.94)

    FSHist = ZFile.Get("emu_type1MET").Rebin(len(binning)-1,"rebin_MET_FS",binning)
    FSHist.Scale(1.1236 * 0.065)

    Z_hist.Add(TTZhist,-1)
    Z_hist.Add(VVVhist,-1)
    Z_hist.Add(ZZ4Lhist,-1)
    Z_hist.Add(ZZ2Lhist,-1)
    Z_hist.Add(WZTo2L2Qhist,-1)
    Z_hist.Add(WZTo3LNuhist,-1)
    Z_hist.Add(FSHist,-1)

    gamma_scale = integrate_histograms(gamma_hist)
    gamma_unweighted_scale = integrate_histograms(gamma_hist_unweighted)
    Z_scale = integrate_histograms(Z_hist)
    gamma_hist.Scale(Z_scale/gamma_scale)
    gamma_hist_unweighted.Scale(Z_scale/gamma_unweighted_scale)

    ply.plot_hist(
            data = Z_hist,
            bgs = [gamma_hist],
            legend_labels = ["#gamma + Jets"],
            options = {
                "yaxis_log":True,
                "output_name":os.path.join(output_prefix,year,"data_only_template_hists.pdf"),
                "xaxis_label":"Type 1 MET",
                "legend_smart":False,
                "legend_datalabel":"Z+Jets",
                "xaxis_range":[50,600],
                "yaxis_range":[0.1,1e7],
                "ratio_range":[0.1,1.9],
                "legend_percentageinbox":False,
                "lumi_value":lumi[year],
                "cms_label":"Preliminary",
                "show_bkg_errors":True,
            }
        )

    ply.plot_hist(
                data = Z_hist,
                bgs = [gamma_hist_unweighted],
                legend_labels = ["#gamma + Jets"],
                options = {
                "yaxis_log":True,
                "output_name":os.path.join(output_prefix,year,"data_only_template_hists_unweighted.pdf"),
                "xaxis_label":"Type 1 MET (unweighted)",
                "legend_smart":False,
                "legend_datalabel":"Z+Jets",
                "xaxis_range":[50,600],
                "yaxis_range":[0.1,1e7],
                "ratio_range":[0.1,1.9],
                "legend_percentageinbox":False,
                "lumi_value":lumi[year],
                "cms_label":"Preliminary",
                "show_bkg_errors":True,
                }

            )


