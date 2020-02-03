from __future__ import print_function
import ROOT as r
import numpy as np
import sys,os
import plottery.plottery as ply

plot_prefix = "/home/users/bsathian/public_html/ZMET_plots/threeyears/DoubleLepton/strategy_B/loose_BVeto_v2/s_over_b_hists/"

dataHistFile = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/Final/DoubleLepton/combined/Data.root")

model = sys.argv[1]
susyHistFile = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/fastsim/Final/CV/combined/"+model+".root")
susyHistFile_GenMet = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/fastsim/Final/GenMET/combined/"model+".root")

if "T5ZZ" in model:
    SRs = ["SRA", "SRB","SRC","SRAb","SRBb","SRCb"]
    massSpectrumFile = open("/home/users/bsathian/ZMet/ZMETBabyLooper2017/SMSScans/Spectra/mass_spectrum_T5ZZ.txt","r")
elif "TChiWZ" in model:
    SRs = ["SRVZBoosted","SRVZResolved"]
    massSpectrumFile = open("/home/users/bsathian/ZMet/ZMETBabyLooper2017/SMSScans/Spectra/mass_spectrum_TChiWZ.txt","r")
else:
    print("Wrong model!")
    sys.exit(1)


smasses = []
mGs = []
mLSPs = []
for line in massSpectrumFile:
    smasses.append((line.split()[1],line.split()[3]))
    mGs.append(float(line.split()[1]))
    mLSPs.append(float(line.split()[3]))

mGs = sorted(set(mGs))
mLSPs = sorted(set(mLSPs))
mGs = np.array(mGs,dtype = np.float64)
mLSPs = np.array(mLSPs,dtype = np.float64)
sOverB_hists = []
for SR in SRs:

    sOverB_hists.append(r.TH2D("sOverB_"+SR,"",len(mGs)-1,mGs,len(mLSPs)-1,mLSPs))
    data_ee_hist = dataHistFile.Get(SR+"ee_type1MET").Clone("data_hist_ee")
    data_mumu_hist = dataHistFile.Get(SR+"mumu_type1MET").Clone("data_hist_mumu")
    data_hist = data_ee_hist.Clone("data_hist")
    data_hist.Add(data_mumu_hist)

    firstBinYield = data_hist.Integral(data_hist.FindBin(50),data_hist.FindBin(99.99))

    susy_hist = susyHistFile.Get(SR+"susy_type1MET_counts").Clone("susy_hist")
    for mG,mLSP in smasses:
        mG = float(mG)
        mLSP = float(mLSP)
        signalYield = susy_hist.Integral(susy_hist.GetXaxis().FindBin(50),susy_hist.GetXaxis().FindBin(99.99),susy_hist.GetYaxis().FindBin(mG),susy_hist.GetYaxis().FindBin(mG),susy_hist.GetZaxis().FindBin(mLSP),susy_hist.GetZaxis().FindBin(mLSP))
        signalYield_GenMet = susy_hist_GenMet.Integral(susy_hist_GenMet.GetXaxis().FindBin(50),susy_hist_GenMet.GetXaxis().FindBin(99.99),susy_hist_GenMet.GetYaxis().FindBin(mG),susy_hist_GenMet.GetYaxis().FindBin(mG),susy_hist_GenMet.GetZaxis().FindBin(mLSP),susy_hist_GenMet.GetZaxis().FindBin(mLSP))

        print(mG,mLSP,signalYield/firstBinYield)
        sOverB_hists[-1].Fill(mG,mLSP,0.5 * (signalYield + signalYield_GenMet)/firstBinYield)




#plot the histograms
for idx,hist in enumerate(sOverB_hists):
    hist.Print("all")
    ply.plot_hist_2d(
            hist,
            options = {
                    "zaxis_log":True,
            #        "bin_text_smart":True,
                    "output_name":os.path.join(plot_prefix,model,"s_over_b_"+SRs[idx]+".pdf")
                }
            )
