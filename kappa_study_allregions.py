from __future__ import print_function
import ROOT as r
import numpy as np
import sys,os
import plottery.plottery as ply


prefix_list = ["allSRs","inclusive_btag","inclusive_bveto","inclusive_SRA","inclusive_SRB","inclusive_SRC","strong_btag","strong_bveto","inclusive_MET100150","inclusive_MET150250","inclusive_MET250inf"]
parent_directory = "/home/users/bsathian/ZMet/histsthreeyears/kappa_studies/"
plot_directory="/home/users/bsathian/public_html/ZMET_plots/threeyears/kappa_studies/"
kappa_data = {}
kappa_mc = {}
kappa = None

def compute_kappa(f,prefix,SRs = None):
    global kappa
    if SRs is None:
        SRs = [""]
    for SR in SRs:
        print("SR=",SR)
        try:
            narrowband_hist = f.Get(SR+"count_narrowband").Clone(SR+"count_narrowband_clone")
            wideband_hist = f.Get(SR+"count_wideband").Clone(SR+"count_wideband_clone")
        except:
            if SR != "":
                kappa[SR] = [0.0,0.0]
            else:
                kappa[prefix] = [0.0,0.0]
            continue

        narrowband_hist.Divide(wideband_hist) #inplace division
        if SR != "":
            kappa[SR] = [narrowband_hist.GetBinContent(2),narrowband_hist.GetBinError(2)]
        else:
            kappa[prefix] = [narrowband_hist.GetBinContent(2),narrowband_hist.GetBinError(2)]


keys = ["SRA","SRB","SRC","SRAb","SRBb","SRCb","SRVZBoosted","SRVZResolved","SRHZ","inclusive_btag","inclusive_bveto","inclusive_SRA","inclusive_SRB","inclusive_SRC","strong_btag","strong_bveto","inclusive_MET100150","inclusive_MET150250","inclusive_MET250inf"]


def printLatex(kappa_data,kappa_mc):
    print("\\hline")
    print("Region && $\kappa$ in data && $\kappa$in MC \\\\")
    print("\\hline")

    keys = ["SRA","SRB","SRC","SRAb","SRBb","SRCb","SRVZBoosted","SRVZResolved","SRHZ","inclusive_btag","inclusive_bveto","inclusive_SRA","inclusive_SRB","inclusive_SRC","strong_btag","strong_bveto","inclusive_MET100150","inclusive_MET150250","inclusive_MET250inf"]

    for key in keys:
        data_estimate = kappa_data[key]
        mc_estimate = kappa_mc[key]
        print(key+"&& {:0.3f}$\pm$ {:0.3f} && {:0.3f} $\pm$ {:0.3f} \\\\".format(data_estimate[0],data_estimate[1],mc_estimate[0],mc_estimate[1]))
    print("\\hline")

lumi = {"2016":"35.9","2017":"41.5","2018":"59.8","combined":137.2}

def plot_values(kappa_data,kappa_mc,keys,filename,era):

    #kappa_values need to be indexed properly!
    kappa_mc_hist = r.TH1D("kappa_mc","MC",len(keys),1,len(keys)-1)
    kappa_data_hist = r.TH1D("kappa_data","data",len(keys),1,len(keys)-1)

    for key,value in kappa_mc.items():
        if key in keys:
            kappa_mc_hist.SetBinContent(keys.index(key)+1,value[0])
            kappa_mc_hist.SetBinError(keys.index(key)+1,value[1])

    for key,value in kappa_data.items():
        if key in keys:
            kappa_data_hist.SetBinContent(keys.index(key)+1,value[0])
            kappa_data_hist.SetBinError(keys.index(key)+1,value[1])

    ply.plot_hist(
            bgs = [kappa_mc_hist,kappa_data_hist],
            colors = [r.kGreen-2,r.kBlack],
            options = {
                "canvas_main_rightmargin":0.1,
                "draw_points":True,
                "show_bkg_errors":True,
                "hist_disable_xerrors":True,
                "xaxis_bin_text_labels":keys,
                "draw_points":True,
                "yaxis_range":[0,0.15],
                "legend_percentageinbox":False,
                "output_name":os.path.join(plot_directory,filename+"_"+era+".pdf"),
                "do_stack":False,
                "yaxis_label":"#kappa estimate",
                "cms_label":"Preliminary",
                "lumi_value":lumi[era],
                }
            )

SRs = ["SRA","SRB","SRC","SRAb","SRBb","SRCb","SRVZResolved","SRVZBoosted","SRHZ"]

if len(sys.argv) > 1:
    era = sys.argv[1]
else:
    era = "combined"

for filetype in ["Data","MC"]:
    if filetype == "Data":
        kappa = kappa_data
    else:
        kappa = kappa_mc

    for prefix in prefix_list:
        print("prefix=",prefix)
        f = r.TFile(os.path.join(parent_directory,prefix,era,filetype+".root"))
        compute_kappa(f,prefix,SRs if prefix == "allSRs" else None)

printLatex(kappa_data,kappa_mc)
plot_values(kappa_data,kappa_mc,keys[:9],"kappa_signal_SRs",era)
plot_values(kappa_data,kappa_mc,keys[9:][::-1],"kappa_inclusive_regions",era)
