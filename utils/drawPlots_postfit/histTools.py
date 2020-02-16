import numpy as np

def updateOverUnderflow(hist,xmax):
    overflowBin = hist.FindBin(xmax - 0.0001)
    for idx in range(overflowBin,hist.GetNbinsX()+1):
        hist.SetBinContent(overflowBin,hist.GetBinContent(overflowBin) + hist.GetBinContent(idx+1))
        hist.SetBinError(overflowBin,np.sqrt(hist.GetBinError(overflowBin) ** 2 + hist.GetBinError(idx+1)**2))
        hist.SetBinContent(idx+1,0)
        hist.SetBinError(idx+1,0)


def hist_sort(hist):
    return hist[0].Integral(0,-1)

