from __future__ import print_function
import ROOT as r
from histTools import updateOverUnderflow,hist_sort
import numpy as np

PALLETE = [r.kSpring-6,r.kAzure+7,r.kRed-7]

def assignColor(hists):
    for idx,hist in enumerate(hists):
        hist.SetFillColor(PALLETE[idx%len(PALLETE)])
        hist.SetFillStyle(1001)



def drawPostfitHistograms(zjets,mcbkg,fsbkg,total,data,binning,SR,output_path = None):
    c = r.TCanvas()
    c.cd()
    r.gStyle.SetOptStat(r.kFALSE)
    fullpad = r.TPad("fullpad"+SR,"fullpad",0,0,1,1)
    fullpad.Draw()
    fullpad.cd()
    plotpad = r.TPad("plotpad"+SR,"plotpad",0,0.2,1,1)
    plotpad.SetRightMargin(0.05)
    plotpad.Draw()
    plotpad.cd()
    plotpad.SetLogy()



    #Data histogram
    assignColor([zjets,fsbkg,mcbkg])
    data.SetMarkerStyle(20)
    data.SetMarkerColor(r.kBlack)

    ymax = 12 * data.GetMaximum()
    ymin = 0.005
    xmin = binning.min()
    xmax = binning.max()

    for hist in [zjets,mcbkg,fsbkg,total,data]:
        updateOverUnderflow(hist,xmax)



    h_axes = r.TH2F("plot_axes"+SR,"plot axes",data.GetNbinsX(),xmin,xmax,1000,ymin,ymax)
    h_axes.GetXaxis().SetTitle("p^{miss}_{T} [GeV]")
    h_axes.GetYaxis().SetTitle("Events")

    r.gStyle.SetTitleW(0.6)
    r.gStyle.SetTitleH(0.06)
    r.gStyle.SetTitleFont(12)



    plotpad.SetLeftMargin(0.12)
    plotpad.SetTopMargin(0.08)
    h_axes.GetYaxis().SetTitleOffset(1.0)
    h_axes.GetYaxis().SetTitleSize(0.05)
    h_axes.GetYaxis().SetLabelSize(0.04)
    h_axes.GetXaxis().SetTitleSize(0.05)
    h_axes.GetXaxis().SetLabelSize(0.04)
    h_axes.Draw()

    #Errors for the hatchband. Use the total histogram for this purpose

    #bgs_syst = total.Clone("bgs_syst"+SR)
    # The whole error shebang
    total_counts = []
    total_errors = []
    bin_left = []
    bin_width = []

    for i in range(1,total.GetNbinsX()+1):
        total_counts.append(total.GetBinContent(i))
        total_errors.append(total.GetBinError(i))
        bin_left.append(binning[i-1])
        bin_width.append(binning[i] - binning[i-1])

    total_counts = np.array(total_counts,dtype = np.float64)
    total_errors = np.array(total_errors,dtype = np.float64)
    ratio_counts = total_counts/total_counts
    ratio_errors = total_errors/total_counts

    bin_left = np.array(bin_left,dtype = np.float64)
    bin_width = np.array(bin_width,dtype = np.float64)/2
    bin_left += bin_width
    bgs_syst = r.TGraphErrors(len(bin_left),bin_left,total_counts,bin_width,total_errors)

    bgs_syst.SetFillColor(r.kGray+3)
    bgs_syst.SetFillStyle(3244)

    ratio_syst = total.Clone("ratio_syst")

    #Stack the histograms
    stack = r.THStack("stack_MET"+SR,"MET stack")

    hist_ordered = sorted([(zjets,"Z+Jets"),(fsbkg,"Flavor Symmetric"),(mcbkg,"Rare MC")],key = hist_sort)#, reverse = True)

    for hist in hist_ordered:
        stack.Add(hist[0])

    stack.Draw("HIST SAME")
    data.SetMarkerSize(0.8)
    data.SetLineColor(r.kBlack)
    data.SetMarkerColor(r.kBlack)
    data.SetLineWidth(2)
    data.Draw("same e0 x0 e1 p0")
    bgs_syst.Draw("e2 same")

    plotpad.RedrawAxis()

    #Legend
    l1 = r.TLegend(0.63,0.67,0.93,0.87)
    l1.SetTextFont(42)
    l1.SetLineColor(r.kWhite)
    l1.SetShadowColor(r.kWhite)
    l1.SetFillColor(r.kWhite)
    l1.SetTextSize(0.03)

    l1.AddEntry(data,"Data","pe")
    for hist in hist_ordered[::-1]:
        l1.AddEntry(hist[0],hist[1],"f")

    l1.Draw("same")
    c.Update()

    #Ratio band - use the background hist!!

    fullpad.cd()
    ratiopad = r.TPad("ratiopad"+SR,"ratiopad",0,0.02,1,0.18)
    ratiopad.SetTopMargin(0.05)
    ratiopad.SetLeftMargin(0.12)
    ratiopad.SetBottomMargin(0.21)
    ratiopad.SetRightMargin(0.05)
    ratiopad.SetGridy()
    ratiopad.Draw()
    ratiopad.cd()

    residual = data.Clone("residual")
    totalNoError = total.Clone("total_no_error"+SR)

    for i in range(1,totalNoError.GetNbinsX()):
        totalNoError.SetBinError(i,0)

    residual.Divide(totalNoError)

    h_axis_ratio = r.TH1D(SR+"residual_axes","",residual.GetNbinsX(),xmin,xmax)
    h_axis_ratio.SetMarkerStyle(20)
    h_axis_ratio.SetMarkerSize(0.8)
    h_axis_ratio.SetLineWidth(2)
    h_axis_ratio.GetYaxis().SetTitleOffset(0.33)
    h_axis_ratio.GetYaxis().SetTitleSize(0.16)
    h_axis_ratio.GetYaxis().SetNdivisions(505)
    h_axis_ratio.GetYaxis().SetLabelSize(0.15)

    if residual.GetMaximum() > 3 and residual.GetMaximum() <= 4:
        h_axis_ratio.GetYaxis().SetRangeUser(0.001,4.0)
    elif residual.GetMaximum() > 2 and residual.GetMaximum() <= 3:
        h_axis_ratio.GetYaxis().SetRangeUser(0.001,3.0)
    else:
        h_axis_ratio.GetYaxis().SetRangeUser(0.001,2.0)

    h_axis_ratio.GetYaxis().SetTitle("Data/MC")

    h_axis_ratio.GetXaxis().SetTickLength(0.07)
    h_axis_ratio.GetXaxis().SetTitleSize(0.005)
    h_axis_ratio.GetXaxis().SetLabelSize(0.)

    h_axis_ratio.GetYaxis().CenterTitle()

    ratio_one_line = r.TLine(xmin,1,xmax,1)
    ratio_one_line.SetLineStyle(1)
    residual.SetMarkerSize(0.8)

    h_axis_ratio.Draw("axis")

    ratio_syst = r.TGraphErrors(len(bin_left),bin_left,ratio_counts,bin_width,ratio_errors)
    ratio_syst.SetFillColorAlpha(r.kGray+3,0.4)
    ratio_syst.SetFillStyle(1001)

    ratio_one_line.Draw("same")
    ratio_syst.Draw("SAME 2")
    residual.Draw("same PE e0 x0 e1 p0")
    #TODO:ratio errors
    c.Update()
    c.cd()


    xcms = r.gPad.GetLeftMargin() - 0.025
    ycms = 0.99 - r.gPad.GetTopMargin()
    xlumi = 0.975 - r.gPad.GetRightMargin()

    lumitex = r.TLatex(xlumi,ycms,"137.2 fb^{-1} (13 TeV)")
    lumitex.SetTextAlign(31)
    lumitex.SetNDC()
    lumitex.SetTextSize(0.035)
    lumitex.SetTextFont(42)
    lumitex.Draw();

    cmstexbold = r.TLatex(xcms,ycms,"#scale[1.25]{#font[61]{CMS}} #scale[1.1]{#font[52]{Preliminary}}")
    cmstexbold.SetTextAlign(11)
    cmstexbold.SetTextColor(r.kBlack)
    cmstexbold.SetTextSize(0.035)
    cmstexbold.SetNDC()
    cmstexbold.Draw()

    printLatexTable(zjets,mcbkg,fsbkg,total,data,binning,SR)
    if output_path:
        c.SaveAs(output_path)
    else:
        print("Output path not given. Saving as plot.pdf")
        c.SaveAs("plot.pdf")


def printLatexTable(zjets,mcbkg,fsbkg,total,data,binning,SR):
    print("\nLATEXTABLE")
    print("{} \MET ".format(SR),end = "")
    for i in range(len(binning)-1):
        print("& {}-{}".format(binning[i],binning[i+1]),end = "")
    print("\\\\")

    print("ZJets",end = "")
    for i in range(len(binning)-1):
        print("& {:0.1f} $\pm$ {:0.1f}".format(zjets.GetBinContent(i+1),zjets.GetBinError(i+1)),end = "")
    print("\\\\")

    print("FS", end = "")
    for i in range(len(binning)-1):
        print("& {:0.1f} $\pm$ {:0.1f}".format(fsbkg.GetBinContent(i+1),fsbkg.GetBinError(i+1)),end = "")
    print("\\\\")

    print("Rares",end = "")
    for i in range(len(binning)-1):
        print("& {:0.1f} $\pm$ {:0.1f}".format(mcbkg.GetBinContent(i+1),mcbkg.GetBinError(i+1)),end = "")
    print("\\\\")

    print("Total",end = "")
    for i in range(len(binning)-1):
        print("& {:0.1f} $\pm$ {:0.1f}".format(total.GetBinContent(i+1),total.GetBinError(i+1)),end  = "")
    print("\\\\")
    print("Data",end = "")
    for i in range(len(binning)-1):
        print("& {:0.1f} $\pm {:0.1f}".format(data.GetBinContent(i+1),data.GetBinError(i+1)),end = "")

    print("\\\\\n")





