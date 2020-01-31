from __future__ import print_function
import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os

prefix = "/home/users/bsathian/ZMet/histsthreeyears/strategy_B/loose_BVeto/MCTemplates/combined/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/threeyears/MCTemplates/strategy_B/loose_BVeto/"
gammaFile = r.TFile(os.path.join(prefix,"GammaJets_reweight.root"))
gammaFile_unweighted = r.TFile(os.path.join(prefix,"GammaJets.root"))
ZFile = r.TFile(os.path.join(prefix,"ZJets.root"))

SRs = ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"]

def integrate_histograms(hist):
    scale = hist.Integral(hist.FindBin(50),hist.FindBin(99.99))
    return scale

SRC_binning = np.array([0,50,100,150,250,600],dtype = np.float64)
SRVZBoosted_binning = np.array([0,50,100,600],dtype = np.float64)
SRVZResolved_binning = np.array([0,50,100,150,250,600],dtype = np.float64)
SRHZ_binning = np.array([0,50,100,150,600],dtype = np.float64)
SRB_binning = np.array([0,50,100,150,230,600],dtype = np.float64)
other_binning = np.array([0,50,100,150,230,300,600],dtype = np.float64)
for SR in SRs:
    print("SR =",SR)
    if "SRB" in SR or "VRB" in SR:
        binning = SRB_binning
    elif  "SRA" in SR  or "VRA" in SR:
        binning = other_binning
    elif "SRC" in SR or "VRC" in SR:
        binning = SRC_binning
    elif "Boosted" in SR:
        binning = SRVZBoosted_binning
    elif "Resolved" in SR:
        binning = SRVZResolved_binning
    else:
        binning = SRHZ_binning
#    gamma_hist = gammaFile.Get(SR + "type1MET_widebin"
    gamma_hist = gammaFile.Get(SR + "type1MET").Rebin(len(binning)-1,"rebin_met_mc_"+SR,binning)
    gamma_hist_unweighted = gammaFile_unweighted.Get(SR+"type1MET").Rebin(len(binning)-1,"rebin_met_mc_unsweighted_"+SR,binning)
    gamma_Pt_unweighted = gammaFile_unweighted.Get(SR+"vpt")
    gamma_Pt = gammaFile..Get(SR+"vpt")
    Z_Pt = ZFile.Get(SR+"vpt")
    Z_hist = ZFile.Get(SR+"type1MET").Rebin(len(binning)-1,"rebin_met_Z_mc_"+SR,binning)
    gamma_scale = integrate_histograms(gamma_hist)
    gamma_unweighted_scale = integrate_histograms(gamma_hist_unweighted)
    Z_scale = integrate_histograms(Z_hist)
    gamma_hist.Scale(Z_scale/gamma_scale)
    gamma_hist_unweighted.Scale(Z_scale/gamma_unweighted_scale)


    gamma_Pt_unweighted.Scale(Z_Pt.Integral(0,-1)/gamma_Pt_unweighted.Integral(0,-1))
    gamma_Pt.Scale(Z_Pt.Integral(0,-1)/gamma_Pt.Integral(0,-1))
    ply.plot_hist(
            data = Z_hist,
            bgs = [gamma_hist],
            legend_labels = ["#gamma + Jets"],
            options = {
                "yaxis_log":True,
                "output_name":output_prefix+SR+"_template_hists.pdf",
                "xaxis_label":"Type 1 MET",
                "title" : SR,
                "legend_smart":False,
                "legend_datalabel":"Z+Jets",
                "xaxis_range":[50,600],
                "yaxis_range":[0.1,1e7],
                "ratio_range":[0.1,1.9],
                "legend_percentageinbox":False,
                "lumi_value":137.2,
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
                "output_name":output_prefix+SR+"_template_hists_unweighted.pdf",
                "xaxis_label":"Type 1 MET (unweighted)",
                "legend_smart":False,
                "legend_datalabel":"Z+Jets",
                "xaxis_range":[50,600],
                "yaxis_range":[0.1,1e7],
                "ratio_range":[0.1,1.9],
                "legend_percentageinbox":False,
                "lumi_value":137.2,
                "cms_label":"Preliminary",
                "show_bkg_errors":True,
                }

            )


    ply.plot_hist(
            data = Z_Pt,
            bgs = [gamma_Pt],
            legend_labels = ["#gamma + Jets"],
            options = {
                "yaxis_log":True,
                "output_name":output_prefix+SR+"_boson_pt.pdf",
                "xaxis_label":"Boson P_{T}",
                "legend_smart":False,
                "legend_datalabel":"Z+Jets",
                "xaxis_range":[50,600],
                "yaxis_range":[0.1,1e7],
                "ratio_range":[0.1,1.9],
                "legend_percentageinbox":False,
                "lumi_value":137.2,
                "cms_label":"Preliminary",
                "show_bkg_errors":True,
                }
            )
    #Closure error computing
    ratio_hist = Z_hist.Clone("gamma_hist_for_ratio")
    ratio_hist.Divide(gamma_hist)
    print("\\hline")
    for i in range(2,len(binning)):
        if i == 2:
            SR_toprint = SR
        else:
            SR_toprint = ""
        print("{} & {}-{} & ${:0.2f} \pm {:0.2f}$ & ${:0.2f} \pm {:0.2f}$ & ${:0.2f} \pm {:0.2f}$\\\\".format(SR_toprint,Z_hist.GetBinLowEdge(i),Z_hist.GetBinLowEdge(i+1),gamma_hist.GetBinContent(i),gamma_hist.GetBinError(i),Z_hist.GetBinContent(i),Z_hist.GetBinError(i),ratio_hist.GetBinContent(i),ratio_hist.GetBinError(i)))
#        print("SR = {} Bin {}-{} Gamma = {} \pm {}, Z = {} \pm {}, Ratio = {} Closure Error = {}".format(SR,Z_hist.GetBinLowEdge(i),Z_hist.GetBinLowEdge(i+1),gamma_hist.GetBinContent(i), gamma_hist.GetBinError(i),Z_hist.GetBinContent(i), Z_hist.GetBinError(i),ratio_hist.GetBinContent(i),max(ratio_hist.GetBinError(i),abs(1-ratio_hist.GetBinContent(i)))))
    print("\\hline")
#Plot directory stuff
os.system("chmod -R 755 "+output_prefix)
os.system("sh ~/niceplots/niceplots.sh "+output_prefix)

