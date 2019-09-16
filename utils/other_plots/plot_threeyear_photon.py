import plottery.plottery as ply
import numpy as np
import ROOT as r
import sys

prefix = "/home/users/bsathian/ZMet/histsthreeyears/PhotonCR/combined/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/threeyears/PhotonCR/"
Data_histFile = r.TFile(prefix+"GammaData.root")
MC_histFile = r.TFile(prefix+"GammaJets.root")
EWK_WGamma = r.TFile(prefix+"WGamma-EWKSub.root")
EWK_WJets = r.TFile(prefix+"WJets-EWKSub.root")
EWK_ttbar2lep = r.TFile(prefix+"TTJets-2lep-EWKSub.root")
EWK_ttbar1lep = r.TFile(prefix+"TTJets-1lep-EWKSub.root")
EWK_SingleTop = r.TFile(prefix+"SingleTop-EWKSub.root")

def scale_histograms(mc = None,data = None,MET=False):
    '''Scale MC histogram to match data histogram and return the scale value'''
    if mc is None or data is None:
        raise ValueError("No MC and Data histograms specified")
    if MET:
        scale = data.Integral(data.FindBin(50),data.FindBin(99.99))/mc.Integral(mc.FindBin(50),mc.FindBin(99.99))
    else:
        scale = data.Integral()/mc.Integral()
#    scale = 1.381
    mc.Scale(scale)
    return mc,data,scale


def plot_hist_with_scale(data = None,bgs = [],legend_labels = [],options = {},MET=False):
    index = legend_labels.index("Gamma+Jets")
    bgs[index],data_hist,scale = scale_histograms(bgs[index],data,MET)
    options["extra_text"] = ["Scaled by %.3f"%(scale),]

    if "extra_text_xpos" not in options.keys():
        print("Setting x position of scale text")
        options["extra_text_xpos"] = 0.75
    if "extra_text_ypos" not in options.keys():
        print("Setting y position of scale text")
        options["extra_text_ypos"] = 0.6
    options["extra_text_size"] = 0.03
    options["lumi_value"] = "41.5"
    options["show_bkg_errors"] = True
    ply.plot_hist(
            data = data_hist,
            bgs = bgs,
            legend_labels = legend_labels,
            options = options
            )


params = ["type1MET","vpt_flat","photonEta","ht","njets","softDropMass","nFatJets","tau21"]
rebin_values = [50,10,10,10,1,10,1,1]
binning = np.array([0,50,100,150,230,300,600],dtype = np.float64)
xaxis_ranges = [[50,600],[0,1000],[-2.4,2.4],[0,1000],[0,10],[0,200],[0,10],[0,1]]
for SR in ["SRA","SRB","SRC","SRAb","SRBb","SRCb","SRVZResolved","SRVZBoosted","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"]:
    for index,param in enumerate(params):
        if (param == "softDropMass" or param == "nFatJets" or param == "tau21") and "Boosted" not in SR:
            continue
        MC_hists = {}
        rebin = rebin_values[index]
        hist_data = Data_histFile.Get(SR+param)
        if "MET" in param:
            hist_data = hist_data.Rebin(len(binning)-1,"type1MET_data",binning)
            print("MET binning of data!")
        else:
            hist_data = hist_data.Rebin(rebin)

        hist_MC = MC_histFile.Get(SR+param)
        if type(hist_MC) is r.TH1D:
            if "MET" in param:
                hist_MC = hist_MC.Rebin(len(binning)-1,"type1MET_MC",binning)
                print("MET binning of MC")
            else:
                hist_MC.Rebin(rebin)
            MC_hists["Gamma+Jets"] = hist_MC

        hist_WGamma = EWK_WGamma.Get(SR+param)
        if type(hist_WGamma) is r.TH1D:
            if "MET" in param:
                hist_WGamma = hist_WGamma.Rebin(len(binning)-1,"type1MET_WGamma",binning)
                print("MET binning of WGamma")
            else:
                hist_WGamma.Rebin(rebin)
            hist_WGamma.Scale(-1)
            MC_hists["WGamma"] = hist_WGamma

        hist_WJets = EWK_WJets.Get(SR+param)
        if type(hist_WJets) is r.TH1D:
            if "MET" in param:
                hist_WJets = hist_WJets.Rebin(len(binning)-1,"type1MET_WJets",binning)
                print("MET binning of WJets")
            else:
                hist_WJets.Rebin(rebin)
            hist_WJets.Scale(-1)
            MC_hists["WJets"] = hist_WJets

        hist_ttbar2lep = EWK_ttbar2lep.Get(SR+param)
        if type(hist_ttbar2lep) is r.TH1D:
            if "MET" in param:
                hist_ttbar2lep = hist_ttbar2lep.Rebin(len(binning)-1,"type1MET_ttbar2lep",binning)
                print("MET binning of TTBar2lep")
            else:
                hist_ttbar2lep.Rebin(rebin)
            hist_ttbar2lep.Scale(-1)
            MC_hists["tt2lep"] = hist_ttbar2lep

        hist_ttbar1lep = EWK_ttbar1lep.Get(SR+param)
        if type(hist_ttbar1lep) is r.TH1D:
            if "MET" in param:
                hist_ttbar1lep = hist_ttbar1lep.Rebin(len(binning)-1,"type1MET_ttbar1lep",binning)
                print("MET binning of TTBar1lep")
            else:
                hist_ttbar1lep.Rebin(rebin)
            hist_ttbar1lep.Scale(-1)
            MC_hists["tt1lep"] = hist_ttbar1lep

        hist_SingleTop = EWK_SingleTop.Get(SR+param)
        if type(hist_SingleTop) is r.TH1D:
            if "MET" in param:
                hist_SingleTop = hist_SingleTop.Rebin(len(binning)-1,"type1MET_SingleTop",binning)
                print("MET binning of SingleTop")
            else:
                hist_SingleTop.Rebin(rebin)
            hist_SingleTop.Scale(-1)
            MC_hists["Single Top"] = hist_SingleTop


        plot_hist_with_scale(
                data = hist_data,
                bgs = list(MC_hists.values()),
                legend_labels = list(MC_hists.keys()),
                options = {
                    "yaxis_log":True,
                    "xaxis_range":xaxis_ranges[index],
                    "yaxis_range":[0.1,2e6],
                    "output_name":output_prefix+SR+"_"+param+".pdf",
                    "ratio_range":[0.5,1.5],
                    "xaxis_label":param,
                    "lumi_value":137.2,
                    "legend_smart":False,
                    "legend_percentageinbox":False,
                    "cms_label":"Preliminary",
                    },
                MET = "MET" in param
                )


os.system("chmod -R 755 "+output_prefix)
os.system("sh ~/niceplots/niceplots.sh "+output_prefix)



