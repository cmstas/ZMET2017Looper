from __future__ import print_function
import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os

prefix = "/home/users/bsathian/ZMet/histsthreeyears/EWKCR/combined/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/threeyears/EWKCR/"

data_file = r.TFile(os.path.join(prefix,"MuonData.root"))
gamma_mc_file = r.TFile(os.path.join(prefix,"GammaJets.root"))
ttjets_2lep_file = r.TFile(os.path.join(prefix,"TTJets-2lep.root"))
ttjets_1lep_file = r.TFile(os.path.join(prefix,"TTJets-1lep.root"))
singletop_file = r.TFile(os.path.join(prefix,"SingleTop.root"))
wgamma_file = r.TFile(os.path.join(prefix,"WGamma.root"))
wjets_file = r.TFile(os.path.join(prefix,"WJets.root"))


#rebinning
MET_binning = np.array([50,100,150,250,350,600],dtype = np.float64)
vpt_binning = np.array([0,25,50,75,100,150,200,250,300,400,500,750,1000],dtype = np.float64)
params = ["type1MET","vpt_flat"]

for index,param in enumerate(params):
    data_hist = data_file.Get(param)
    gamma_mc_hist = gamma_mc_file.Get(param)
    ttjets_2lep_hist = ttjets_2lep_file.Get(param)
    ttjets_1lep_hist = ttjets_1lep_file.Get(param)
    singletop_hist = singletop_file.Get(param)
    wgamma_hist = wgamma_file.Get(param)
    wjets_hist = wjets_file.Get(param)
    if param == "type1MET":
        binning = MET_binning
    elif param == "vpt_flat":
        binning = vpt_binning
    else:
        binning = MET_binning

    data_hist = data_hist.Rebin(len(binning)-1,"rebin_data",binning)
    gamma_mc_hist = gamma_mc_hist.Rebin(len(binning)-1,"rebin_gammamc",binning)
    ttjets_2lep_hist = ttjets_2lep_hist.Rebin(len(binning)-1,"rebin_2lep",binning)
    ttjets_1lep_hist = ttjets_1lep_hist.Rebin(len(binning)-1,"rebin_1lep",binning)
    singletop_hist = singletop_hist.Rebin(len(binning)-1,"rebin_singletop",binning)
    wgamma_hist = wgamma_hist.Rebin(len(binning)-1,"rebin_wgamma",binning)
    wjets_hist = wjets_hist.Rebin(len(binning)-1,"rebin_wjets",binning)


    #MET histograms
    ply.plot_hist(
            data = data_hist,
            bgs = [gamma_mc_hist,ttjets_2lep_hist,ttjets_1lep_hist,singletop_hist,wgamma_hist,wjets_hist],
            legend_labels = ["#gamma + Jets","t#bar{t}#rightarrow 2lep","t#bar{t}#rightarrow 1lep","single top","W+#gamma","W + Jets"],
            options = {
                "yaxis_log":True,
                "output_name":output_prefix+param+".pdf",
                "legend_smart":False,
                "legend_datalabel":"Single #mu Data",
#                "xaxis_range":,
                "yaxis_range":[0.1,3e4],
                "ratio_range":[0.5,1.5],
                "legend_percentageinbox":False,
                "lumi_value":137.2,
                "cms_label":"Preliminary",
                "show_bkg_errors":True,
                }
            )

