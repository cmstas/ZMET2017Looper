import ROOT as r
import plottery.plottery as ply
import numpy as np
import sys,os

FS_file = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/Final/DoubleLepton/combined/TTJets.root")

prefix_path = "/home/users/bsathian/public_html/ZMET_plots/threeyears/FSClosure/"

strong_SR_binning = np.array([50,100,150,230,300,600],dtype = np.float64)
SRC_binning = np.array([50,100,150,250,600],dtype = np.float64)
SRVZBoosted_binning = np.array([50,100,200,300,400,500,600],dtype = np.float64)
SRVZResolved_binning = np.array([50,100,150,250,350,600],dype = np.float64)
SRHZ_binning = np.array([50,100,150,250,600],dtype = np.float64)

kappa_values = {
        "SRA":0.064,
        "SRB":0.067,
        "SRC":0.038,
        "SRVZBoosted":0.055,
        "SRVZResolved":0.062,
        "SRHZ":0.045,
        }

for SR in ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ"]:
    data_ee_hist = FS_file.Get(SR+"ee_type1MET").Clone("ee_type1MET")
    data_mumu_hist = FS_file.Get(SR+"mumu_type1MET").Clone("mumu_type1MET")
    data_hist = data_ee_hist.Clone(SR+"data_type1MET")
    data_hist.Add(data_mumu_hist)
    if "SRVZBoosted" in SR:
        binning = SRVZBoosted_binning
    elif "SRVZResolved" in SR:
        binning = SRVZResolved_binning
    elif "SRHZ" in SR:
        binning = SRHZ_binning
    elif "SRC" in SR:
        binning = SRC_binning
    else:
        binning =strong_SR_binning
    data_hist = data_hist.Rebin(len(binning),SR+"data_type1MET_rebinned",binning)

    mc_hist = FS_file.Get(SR+"emu_FS_type1MET").Clone(SR+"emu_type1MET")
    mc_hist = mc_hist.Rebin(len(binning)-1,SR+"mc_met_rebinned",binning)
    if SR[-1] == "b":
        kappa = kappa_values[SR[:-1]]
    else:
        kappa = kappa_values[SR]
    mc_hist.Scale(kappa)

    ply.plot_hist(
            data = data_hist,
            mc = [mc_hist],
            legend_labels = ["MC e#mu"],
            options = {
                "yaxis_log":False,
                "yaxis_range":[1e-3,100],
                "legend_percentageinbox":False,
                "output_name":os.path.join(prefix_path,SR+"FSClosure.pdf")
                }
            )
