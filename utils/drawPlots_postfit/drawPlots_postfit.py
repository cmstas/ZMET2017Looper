import ROOT as r
import numpy as np
import sys,os
import argparse
from plotTools import drawPostfitHistograms
def setTDRStyle():
    tdrStyle = r.TStyle("tdrStyle","Style for P-TDR");

    # For the canvas:
    tdrStyle.SetCanvasBorderMode(0)
    tdrStyle.SetCanvasColor(r.kWhite)
    tdrStyle.SetCanvasDefH(600) #Height of canvas
    tdrStyle.SetCanvasDefW(600) #Width of canvas
    tdrStyle.SetCanvasDefX(0);   #Position on screen
    tdrStyle.SetCanvasDefY(0);

    # For the Pad:
    tdrStyle.SetPadBorderMode(0)
    tdrStyle.SetPadColor(r.kWhite)
    tdrStyle.SetPadGridX(False)
    tdrStyle.SetPadGridY(False)
    tdrStyle.SetGridColor(0)
    tdrStyle.SetGridStyle(3)
    tdrStyle.SetGridWidth(1)

    # For the frame:
    tdrStyle.SetFrameBorderMode(0)
    tdrStyle.SetFrameBorderSize(1)
    tdrStyle.SetFrameFillColor(0)
    tdrStyle.SetFrameFillStyle(0)
    tdrStyle.SetFrameLineColor(1)
    tdrStyle.SetFrameLineStyle(1)
    tdrStyle.SetFrameLineWidth(1)

    # For the histo:
    tdrStyle.SetHistLineColor(1)
    tdrStyle.SetHistLineStyle(0)
    tdrStyle.SetHistLineWidth(1)

    tdrStyle.SetEndErrorSize(2)
    tdrStyle.SetErrorX(0.0)

    tdrStyle.SetMarkerStyle(20)

    #For the fit/function:
    tdrStyle.SetOptFit(1)
    tdrStyle.SetFitFormat("5.4g")
    tdrStyle.SetFuncColor(2)
    tdrStyle.SetFuncStyle(1)
    tdrStyle.SetFuncWidth(1)


    tdrStyle.SetOptFile(0)
    tdrStyle.SetOptStat(0)
    tdrStyle.SetStatColor(r.kWhite)
    tdrStyle.SetStatFont(42)
    tdrStyle.SetStatFontSize(0.025)
    tdrStyle.SetStatTextColor(1)
    tdrStyle.SetStatFormat("6.4g")
    tdrStyle.SetStatBorderSize(1)
    tdrStyle.SetStatH(0.1)
    tdrStyle.SetStatW(0.15)

    #Margins:
    tdrStyle.SetPadTopMargin(0.05)
    tdrStyle.SetPadBottomMargin(0.13)
    tdrStyle.SetPadLeftMargin(0.16)
    tdrStyle.SetPadRightMargin(0.02)

    tdrStyle.SetOptTitle(0)
    tdrStyle.SetTitleFont(42)
    tdrStyle.SetTitleColor(1)
    tdrStyle.SetTitleTextColor(1)
    tdrStyle.SetTitleFillColor(10)
    tdrStyle.SetTitleFontSize(0.035)

    # For the axis titles:
    tdrStyle.SetTitleColor(1, "XYZ")
    tdrStyle.SetTitleFont(42, "XYZ")
    tdrStyle.SetTitleSize(0.06, "XYZ")
    tdrStyle.SetTitleXOffset(0.9)
    tdrStyle.SetTitleYOffset(1.25)

    # For the axis labels:
    tdrStyle.SetLabelColor(1, "XYZ")
    tdrStyle.SetLabelFont(42, "XYZ")
    tdrStyle.SetLabelOffset(0.007, "XYZ")
    tdrStyle.SetLabelSize(0.05, "XYZ")

    # For the axis:
    tdrStyle.SetAxisColor(1, "XYZ")
    tdrStyle.SetStripDecimals(r.kTRUE)
    tdrStyle.SetTickLength(0.03, "XYZ")
    tdrStyle.SetNdivisions(510, "XYZ")
    tdrStyle.SetPadTickX(1)
    tdrStyle.SetPadTickY(1)

    # Change for log plots:
    tdrStyle.SetOptLogx(0)
    tdrStyle.SetOptLogy(0)
    tdrStyle.SetOptLogz(0)

    #Postscript options:
    tdrStyle.SetPaperSize(20.,20.)

    tdrStyle.cd()
    return tdrStyle



SR_binning = {
        "SRA":np.array([50,100,150,230,300,600], dtype = np.float64),
        "SRAb":np.array([50,100,150,230,300,600], dtype = np.float64),
        "SRB": np.array([50,100,150,230,300,600], dtype = np.float64),
        "SRBb":np.array([50,100,150,230,300,600], dtype = np.float64),
        "SRC":np.array([50,100,150,250,600], dtype = np.float64),
        "SRCb":np.array([50,100,150,250,600], dtype = np.float64),
        "SRVZBoosted": np.array([50,100,200,300,600], dtype = np.float64),
        "SRVZResolved":np.array([50,100,150,250,350,600], dtype = np.float64),
        "SRHZ":np.array([50,100,150,250,600], dtype = np.float64)
        }
SR_index = {"SRA":1,"SRAb":2,"SRB":3,"SRBb":4,"SRC":5,"SRCb":6}


def drawPlots_postFit(fit_param_path,SR,dataHistPath,output_path = None):
    global SR_binning,SR_index
    ''' Get the appropriate histogram from the fitParams file'''
    try:
        binning = SR_binning[SR]
    except:
        print("Wrong SR")
        sys.exit(1)
    #Make the histograms
    zjets = r.TH1D(SR+"zjets","",len(binning)-1,binning)
    zjets.Sumw2()
    fsbkg = r.TH1D(SR+"fsbkg","",len(binning)-1,binning)
    fsbkg.Sumw2()
    mcbkg = r.TH1D(SR+"mcbkg","",len(binning)-1,binning)
    mcbkg.Sumw2()
    total = r.TH1D(SR+"total","",len(binning)-1,binning)
    total.Sumw2()

    #open the fitParams file and roll it on!
    fitParamsFile = r.TFile(fit_param_path)

    for idx,binEdge in enumerate(binning[:-1],1):
        try:
            binDirectory = fitParamsFile.Get("shapes_fit_b/ch{}_{}_bin{}".format(SR_index[SR],SR,idx))
        except:
            print("Cannot find the appropriate bin number {} ! Did you specify the correct number of bins?".format(idx))
            sys.exit(1)
        try:
            zjets.SetBinContent(zjets.FindBin(binEdge),binDirectory.Get("zjets").GetBinContent(1))
            zjets.SetBinError(zjets.FindBin(binEdge),binDirectory.Get("zjets").GetBinError(1))
        except:
            print("Cannot find the zjets background for bin {}. Substituting with zero instead".format(idx))
            zjets.SetBinContent(zjets.FindBin(binEdge),0.0)
            zjets.SetBinError(zjets.FindBin(binEdge),0.0)

        try:
            fsbkg.SetBinContent(fsbkg.FindBin(binEdge),binDirectory.Get("fsbkg").GetBinContent(1))
            fsbkg.SetBinError(fsbkg.FindBin(binEdge),binDirectory.Get("fsbkg").GetBinError(1))
        except:
            print("Cannot find the fsbkg background for bin {}. Substituting with zero instead".format(idx))
            fsbkg.SetBinContent(fsbkg.FindBin(binEdge),0.0)
            fsbkg.SetBinError(fsbkg.FindBin(binEdge),0.0)


        try:
            mcbkg.SetBinContent(mcbkg.FindBin(binEdge),binDirectory.Get("mcbkg").GetBinContent(1))
            mcbkg.SetBinError(mcbkg.FindBin(binEdge),binDirectory.Get("mcbkg").GetBinError(1))
        except:
            print("Cannot find the mcbkg background for bin {}. Substituting with zero instead".format(idx))
            mcbkg.SetBinContent(mcbkg.FindBin(binEdge),0.0)
            mcbkg.SetBinError(mcbkg.FindBin(binEdge),0.0)


        total.SetBinContent(total.FindBin(binEdge),binDirectory.Get("total_background").GetBinContent(1))
        total.SetBinError(total.FindBin(binEdge),binDirectory.Get("total_background").GetBinError(1))

    #Get the data histogram
    dataHistFile = r.TFile(dataHistPath)
    data_hist = dataHistFile.Get(SR+"ee_type1MET").Clone(SR+"data_hist")
    data_mm_hist = dataHistFile.Get(SR+"mumu_type1MET").Clone(SR+"data_mm_hist")
    data_hist.Add(data_mm_hist)
    data_hist = data_hist.Rebin(len(binning)-1, "data_hist_rebinned",binning)

    drawPostfitHistograms(zjets = zjets, mcbkg = mcbkg, fsbkg = fsbkg, total = total,data = data_hist,binning =binning,SR = SR,output_path= output_path)


def main():
    tdrStyle = setTDRStyle()

    r.gROOT.ForceStyle()

    r.TGaxis.SetExponentOffset(-0.07,0,"y")
    r.TGaxis.SetExponentOffset(-0.8,-0.07,"x")

    parser = argparse.ArgumentParser()
    parser.add_argument("-b","--bkg",type = str, required = True, help = "path to fitDiagnostics.root file containing the backgrounds")
    parser.add_argument("-d","--data",type = str, required = True, help = "path to Data.root file containing the unblinded data")
    parser.add_argument("-s","--sr",type = str,required = True, help = "signal region")
    parser.add_argument("-o","--output", type = str, help = "output path")

    args = parser.parse_args()
    drawPlots_postFit(args.bkg,args.sr,args.data,args.output)

if __name__ == "__main__":
    main()

