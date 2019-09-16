from __future__ import print_function
import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os

prefix = "/home/users/bsathian/ZMet/histsoneyear/PhotonCR/"
output_prefix = "/home/users/bsathian/public_html/ZMET_plots/oneyear"  #ORGANIZED THE OTHER WAY ROUND, ADD MCTemplates while saving!

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
    options["show_bkg_errors"] = True
    ply.plot_hist(
            data = data_hist,
            bgs = bgs,
            legend_labels = legend_labels,
            options = options
            )


params = ["type1MET","vpt_flat","photonEta","ht","njets","softDropMass","nFatJets","tau21","gamma_genpt"]
original_binning = [(0,0,0),(6000,0,6000),(200,-2.4,2.4),(6000,0,6000),(50,0,50),(6000,0,6000),(50,0,50),(1000,0,10),(6000,0,6000)]
rebin_values = [50,10,10,10,1,10,1,1,10]
binning = np.array([0,50,100,150,230,300,600],dtype = np.float64)
xaxis_ranges = [[50,600],[0,1000],[-2.4,2.4],[0,1000],[0,10],[0,200],[0,10],[0,1],[0,1000]]

SRs = ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"]

years = ["2016","2017","2018"]
lumi = {"2016":"35.9","2017":"41.5","2018":"59.8"}
for year in years:
    Data_histFile = r.TFile(os.path.join(prefix,year,"GammaData.root"))
    MC_histFile = r.TFile(os.path.join(prefix,year,"GammaJets.root"))
    EWK_WGamma = r.TFile(os.path.join(prefix,year,"WGamma-EWKSub.root"))
    EWK_WJets = r.TFile(os.path.join(prefix,year,"WJets-EWKSub.root"))
    EWK_ttbar2lep = r.TFile(os.path.join(prefix,year,"TTJets-2lep-EWKSub.root"))
    EWK_ttbar1lep = r.TFile(os.path.join(prefix,year,"TTJets-1lep-EWKSub.root"))
    EWK_SingleTop = r.TFile(os.path.join(prefix,year,"SingleTop-EWKSub.root"))

    for SR in SRs:
        for index,param in enumerate(params):
            if (param == "softDropMass" or param == "nFatJets" or param == "tau21") and "Boosted" not in SR:
                continue
            MC_hists = {}
            rebin = rebin_values[index]
            hist_data = Data_histFile.Get(SR+param)
            if param == "gamma_genpt":
                print("gen pt plots - using vpt_flat for Data")
                hist_data = Data_histFile.Get(SR+"vpt_flat")

            if "MET" in param:
                hist_data = hist_data.Rebin(len(binning)-1,"type1MET_data",binning)
                print("MET binning of data!")
            else:
                hist_data = hist_data.Clone("new_hist_data")
                hist_data.Rebin(rebin)

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
            else:
                print("WGamma not found! creating dummy histogram!")
                if "MET" in param:
                    MC_hists["WGamma"] = r.TH1D("WGamma"+SR+param,"WGamma"+SR+param,len(binning)-1,binning)
                else:
                    temp_hist = r.TH1D("WGamma"+SR+param,"WGamma"+SR+param,original_binning[index][0],original_binning[index][1],original_binning[index][2])
                    temp_hist.Rebin(rebin)
                    MC_hists["WGamma"] = temp_hist



            hist_WJets = EWK_WJets.Get(SR+param)
            if type(hist_WJets) is r.TH1D:
                if "MET" in param:
                    hist_WJets = hist_WJets.Rebin(len(binning)-1,"type1MET_WJets",binning)
                    print("MET binning of WJets")
                else:
                    hist_WJets.Rebin(rebin)
                hist_WJets.Scale(-1)
                MC_hists["WJets"] = hist_WJets
            else:
                print("WJets not found! creating dummy histogram!")
                if "MET" in param:
                    MC_hists["WJets"] = r.TH1D("WJets"+SR+param,"WJets"+SR+param,len(binning)-1,binning)
                else:
                    temp_hist = r.TH1D("WJets"+SR+param,"WJets"+SR+param,original_binning[index][0],original_binning[index][1],original_binning[index][2])
                    temp_hist.Rebin(rebin)
                    MC_hists["WJets"] = temp_hist


            hist_ttbar2lep = EWK_ttbar2lep.Get(SR+param)
            if type(hist_ttbar2lep) is r.TH1D:
                if "MET" in param:
                    hist_ttbar2lep = hist_ttbar2lep.Rebin(len(binning)-1,"type1MET_ttbar2lep",binning)
                    print("MET binning of TTBar2lep")
                else:
                    hist_ttbar2lep.Rebin(rebin)
                hist_ttbar2lep.Scale(-1)
                MC_hists["tt2lep"] = hist_ttbar2lep
            else:
                print("ttbar2lep not found! creating dummy histogram!")
                if "MET" in param:
                    MC_hists["tt2lep"] = r.TH1D("type1MET_ttbar2lep","type1MET_ttbar2lep",len(binning)-1,binning)
                else:
                    temp_hist = r.TH1D("ttbar2lep"+SR+param,"ttbar2lep"+SR+param,original_binning[index][0],original_binning[index][1],original_binning[index][2])
                    temp_hist.Rebin(rebin)
                    MC_hists["tt2lep"] = temp_hist

            hist_ttbar1lep = EWK_ttbar1lep.Get(SR+param)
            if type(hist_ttbar1lep) is r.TH1D:
                if "MET" in param:
                    hist_ttbar1lep = hist_ttbar1lep.Rebin(len(binning)-1,"type1MET_ttbar1lep",binning)
                    print("MET binning of TTBar1lep")
                else:
                    hist_ttbar1lep.Rebin(rebin)
                hist_ttbar1lep.Scale(-1)
                MC_hists["tt1lep"] = hist_ttbar1lep
            else:
                print("ttbar1lep not found! creating dummy histogram!")
                if "MET" in param:
                    MC_hists["tt1lep"] = r.TH1D("ttbar1lep"+SR+param,"ttbar1lep"+SR+param,len(binning)-1,binning)
                else:
                    temp_hist = r.TH1D("ttbar1lep"+SR+param,"ttbar1lep"+SR+param,original_binning[index][0],original_binning[index][1],original_binning[index][2])
                    temp_hist.Rebin(rebin)
                    MC_hists["tt1lep"] = temp_hist



            hist_SingleTop = EWK_SingleTop.Get(SR+param)
            if type(hist_SingleTop) is r.TH1D:
                if "MET" in param:
                    hist_SingleTop = hist_SingleTop.Rebin(len(binning)-1,"type1MET_SingleTop",binning)
                    print("MET binning of SingleTop")
                else:
                    hist_SingleTop.Rebin(rebin)
                hist_SingleTop.Scale(-1)
                MC_hists["Single Top"] = hist_SingleTop
            else:
                print("SingleTop not found! creating dummy histogram!")
                if "MET" in param:
                    MC_hists["SingleTop"] = r.TH1D("SingleTop"+SR+param,"SingleTop"+SR+param,len(binning)-1,binning)
                else:
                    temp_hist = r.TH1D("SingleTop"+SR+param,"SingleTop"+SR+param,original_binning[index][0],original_binning[index][1],original_binning[index][2])
                    temp_hist.Rebin(rebin)
                    MC_hists["SingleTop"] = temp_hist



            plot_hist_with_scale(
                data = hist_data,
                bgs = list(MC_hists.values()),
                legend_labels = list(MC_hists.keys()),
                options = {
                    "yaxis_log":True,
                    "xaxis_range":xaxis_ranges[index],
                    "yaxis_range":[0.1,2e6],
                    "output_name":os.path.join(output_prefix,year,"PhotonCR",SR+"_"+param+".pdf"),
                    "ratio_range":[0.5,1.5],
                    "xaxis_label":param,
                    "lumi_value":lumi[year],
                    "legend_smart":False,
                    "legend_percentageinbox":False,
                    "cms_label":"Preliminary",
                    },
                MET = "MET" in param
                )
    os.system("sh ~/niceplots/niceplots.sh "+os.path.join(output_prefix,year,"PhotonCR"))
    os.system("chmod -R 755 "+os.path.join(output_prefix,year,"PhotonCR"))
    os.system("chmod -R 755 "+os.path.join(output_prefix,year,"PhotonCR")+"/*")

