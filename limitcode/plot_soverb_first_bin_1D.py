from __future__ import print_function
import ROOT as r
import numpy as np
import sys,os
import plottery.plottery as ply

plot_prefix = "/home/users/bsathian/ZMet/public_html/ZMET_plots/threeyears/DoubleLepton/strategy_B/loose_BVeto_v2/s_over_b_hists/"

dataHistFile = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/Final/DoubleLepton/combined/Data.root")

model = sys.argv[1]
susyHistFile = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/fastsim/Final/CV/combined/"+model+".root")
susyHistFile_GenMet = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/fastsim/Final/GenMET/combined/"model+".root")

if "TChiZZ" in model:
    SRs = ["SRVZBoosted","SRVZResolved","SRHZ"]
    massSpectrum = np.arange(100,1301,25,dtype = np.float64)

elif "TChiHZ" in model:
    SRs = ["SRHZ"]
    massSpectrum = np.arange([127,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000,1025,1050,1075,1100,1125,1150,1175,1200,1225,1250,1275,1300],dtype = np.float64)
else:
    print("Wrong model!")
    sys.exit(1)

sOverB_hists = []
for SR in SRs:
    sOverB_hists.append(r.TH1D("sOverB_"+SR,"",len(massSpectrum)-1,massSpectrum))
    data_ee_hist = dataHistFile.Get(SR+"ee_type1MET").Clone("data_hist_ee")
    data_mumu_hist = dataHistFile.Get(SR+"mumu_type1MET").Clone("data_hist_mumu")
    data_hist = data_ee_hist.Clone("data_hist")
    data_hist.Add(data_mumu_hist)

    firstBinYield = data_hist.Integral(data_hist.FindBin(50),data_hist.FindBin(99.99))

    susy_hist = susyHistFile.Get(SR+"susy_type1MET_counts").Clone("susy_hist")
    susy_hist_GenMet = susyHistFile_GenMet.Get(SR+"susy_type1MET_counts").Clone("susy_hist_GenMet")

    for mChi in massSpectrum:
        signalYield_CV = susy_hist.Integral(susy_hist.GetXaxis().FindBin(50),susy_hist.GetXaxis().FindBin(99.99),susy_hist.GetYaxis().FindBin(mChi),susy_hist.GetYaxis().FindBin(mChi))
        signalYield_GenMet = susy_hist_GenMet.Integral(susy_hist_GenMet.GetXaxis().FindBin(50),susy_hist_GenMet.GetXaxis().FindBin(99.99),susy_hist_GenMet.GetYaxis().FindBin(mChi),susy_hist_GenMet.GetYaxis().FindBin(mChi))

        sOverB_hists[-1].Fill(mChi,0.5 * (signalYield + signalYield_GenMet)/firstBinYield)


for idx,hist in enumerate(sOverB_hists):
    ply.plot_hist(
            bgs = [hist],
            options = {
                "yaxis_log":True,
                "output_name":os.path.join(plot_prefix,model,"s_over_b"+SRs[index]+".pdf"),
                "legend_percentageinbox":False,

                }
            )

