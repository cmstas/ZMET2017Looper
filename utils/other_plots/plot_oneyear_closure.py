from __future__ import print_function
import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os

prefix = "/home/users/bsathian/ZMet/histsoneyear/MCTemplates/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/oneyear/"  #ORGANIZED THE OTHER WAY ROUND, ADD MCTemplates while saving!

SRs = ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"]

def integrate_histograms(hist):
    scale = hist.Integral(hist.FindBin(50),hist.FindBin(99.99))
    return scale

binning = np.array([0,50,100,150,230,300,600],dtype = np.float64)
lumi = {"2016":"35.9","2017":"41.5","2018":"59.8"}

for year in ["2016","2017","2018"]:
    gammaFile = r.TFile(os.path.join(prefix,year,"GammaJets_reweight.root"))
    gammaFile_unweighted = r.TFile(os.path.join(prefix,year,"GammaJets.root"))
    ZFile = r.TFile(prefix,"ZJets.root")

    for SR in SRs:
        print("year=",year,"SR=",SR)
        gamma_hist = gammaFile.Get(SR + "type1MET").Rebin(len(binning)-1,"rebin_met_mc_"+SR,binning)
        gamma_hist_unweighted = gammaFile_unweighted.Get(SR+"type1MET").Rebin(len(binning)-1,"rebin_met_mc_unsweighted_"+SR,binning)
        Z_hist = ZFile.Get(SR+"type1MET").Rebin(len(binning)-1,"rebin_met_Z_mc_"+SR,binning)
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
                "output_name":os.path.join(output_prefix,year,"MCTemplates",SR+"_template_hists.pdf"),
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
                "output_name":os.path.join(output_prefix,year,"MCTemplates",SR+"_template_hists_unweighted.pdf"),
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


