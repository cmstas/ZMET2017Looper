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
    line = ""
    while "kmax" not in line:
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
    f.readline()
    line = ""
    while "process" not in line:
        line = f.readline()
#        if "process" in line:
#            break
    #line in consideration has the signal names
    processes = line.rstrip("\n").split("\t")[1:]
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

    if "SRA"in sys.argv[1] or "SRB" in sys.argv[1]:
        bins = np.array([100,150,250,601],dtype = np.float64)
    elif "SRC" in sys.argv[1]:
        bins = np.array([100,150,601],dtype = np.float64)
    elif "Boosted" in sys.argv[1]:
        bins = np.array([100,200,300,400,500,601],dtype = np.float64)
    elif "Resolved" in sys.argv[1]:
        bins = np.array([100,150,250,350,601],dtype = np.float64)

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
    signal_region = sys.argv[1]
    mG = sys.argv[2]
    mLSP = sys.argv[3]
    if "Resolved" in signal_region:
	    f = open ("DataCards/TChiWZ_threeyears_new_binning_strategy_B_20191126/datacard_SRVZResolved_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")
            yaxis_range = [1e-3,5000]
    elif "Boosted" in signal_region:
	    f = open("DataCards/TChiWZ_threeyears_new_binning_strategy_B_20191126/datacard_SRVZBoosted_mGluino_{}_mLSP_{}_.txt".format(mG,mLSP),"r")
            yaxis_range = [1e-3,200]

    hist = makeSignalHistsFromDatacards(f,signal_region,mG,mLSP)
    ply.plot_hist(
            bgs = [hist],
#            legend_labels = ["new datacards from Bobak's babies"],
            legend_labels = [signal_region],
            options ={
                "output_name":"/home/users/bsathian/public_html/ZMET_plots/threeyears/DoubleLepton/strategy_B/signal_plots/TChiWZ_{}_mG_{}_mLSP_{}.pdf".format(signal_region,mG,mLSP),
                "yaxis_log":True,
                "yaxis_range":yaxis_range,
                "legend_percentageinbox":False,
                "title":"{}, Gluino mass={},LSP mass={}".format(signal_region,mG,mLSP),
                "ratio_range":[0.8,1.2]
            }
        )


