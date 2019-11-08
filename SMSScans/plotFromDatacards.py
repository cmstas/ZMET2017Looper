import plottery.plottery as ply
import ROOT as r
import numpy as np
import sys,os
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator,AutoMinorLocator

plt.rcParams["text.usetex"] = True
plt.rcParams["font.family"] = "serif"
plt.rcParams["axes.linewidth"] = 0.5

def makeSignalHistsFromDatacards(f,title,mG,mLSP):
#Get number of bins
    line = f.readline()

    if "imax" in line:
        nBins = int(line.rstrip("\n").split(" ")[1])
    elif "jmax" in line:
        nBgs =  int(line.rstrip("\n").split(" ")[1])

    #Skip the next two lines because we don't want to see data
    f.readline()
    f.readline()
    f.readline()
    f.readline()

    for line in f:
        if "process" in line:
            break
    #line in consideration has the signal names
    processes = line.rstrip("\n").split(" ")[1:]
    signal_values = {}
    #Easier way to remove whitespaces between process names
    processes = np.array(processes)
    processes = processes[(processes != " ") & (processes != "")]
    for process in processes:
        if process not in signal_values.keys():
            signal_values[process] = []

    #Read signal values and fill
    line = f.readline()
    line = f.readline()
    counts = line.rstrip("\n").split(" ")[1:]
    counts = np.array(counts)
    #Removes all the pesky whitespace between counts
    counts = counts[(counts != " ") & (counts != "")]
    for index,count in enumerate(counts):
        signal_values[processes[index]].append(float(count))

    if "SRC" not in sys.argv[1]:
        bins = np.array([100,150,250,601],dtype = np.float64)
    else:
        bins = np.array([100,150,601],dtype = np.float64)

    print("{} signal yield".format(title))
    print(signal_values["sig"])

    hist = r.TH1D("hist_abc",title,len(bins)-1,bins)
    for i,value in enumerate(signal_values["sig"],start = 1):
        hist.SetBinContent(i,value)
        hist.SetBinError(i,0)
    return hist

#    fig,ax = plt.subplots()
#    plt.hist(bins[:-1],bins,weights = signal_values["sig"])
#    plt.yscale("log")
#    ax.xaxis.set_minor_locator(AutoMinorLocator())
#    plt.ylabel(r"Signal Counts")
#    plt.xlabel(r"MET")
#    plt.title(title)

#    plt.ylim(min(1e-3,min(signal_values["sig"]) * 0.75),max(1,max(signal_values["sig"]) * 1.25))
#    plt.ylim(1e-3,100)
#    if "bobak" in title.lower():
#        plt.savefig("signal_plots/sig_hist_mGluino_{}_mLSP_{}_bobak.pdf".format(mG,mLSP))
#    else:
#        plt.savefig("signal_plots/sig_hist_mGluino_{}_mLSP_{}_new.pdf".format(mG,mLSP))


if __name__ == "__main__":
    mG = sys.argv[1]
    mLSP = sys.argv[2]
    #f_bobak = open("DataCards_2016/DataCards/t5zz/datacard_SRA_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")
    f_new = open("../../DataCards/T5ZZ_2016_2016_binning_20191031/datacard_SRA_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")
    f_old = open ("../../DataCards/T5ZZ_2016_2016_binning/datacard_SRA_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")
#    f_new = open("../../DataCards/T5ZZ_2016_2016_binning_old_signal/datacard_SRA_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")

#    bobak_hist = makeSignalHistsFromDatacards(f_bobak,"Signal histogram from Bobak's trees",mG,mLSP)
    old_hist = makeSignalHistsFromDatacards(f_old,"Signal histogram from last week",mG,mLSP)
    new_hist = makeSignalHistsFromDatacards(f_new,"Signal histgoram from last week",mG,mLSP)
    ply.plot_hist(
            data = old_hist,
            bgs = [new_hist],
#            legend_labels = ["new datacards from Bobak's babies"],
            legend_labels = ["New datacards from recent histograms"],
            options ={
                "output_name":"signal_plots/yields_20191031_v_yields_20191026/comparison_hist_mGluino_{}_mLSP_{}.pdf".format(mG,mLSP),
                "yaxis_log":True,
                "yaxis_range":[1e-3,500],
                "legend_percentageinbox":False,
                "legend_datalabel":"Last week's histograms",
                "title":"Comparison plot, Gluino mass={},LSP mass={}".format(mG,mLSP),
                "ratio_range":[0.8,1.2]
            }
        )


