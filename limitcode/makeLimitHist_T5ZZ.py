import ROOT as r
import numpy as np
import sys,os

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

tdrStyle = setTDRStyle()

r.gROOT.ForceStyle()

#Palette it seems
stops = np.array([0.00, 0.34, 0.61, 0.84, 1.00],dtype = np.float64)
red   = np.array([0.50, 0.50, 1.00, 1.00, 1.00],dtype = np.float64)
green = np.array([0.50, 1.00, 1.00, 0.60, 0.50],dtype = np.float64)
blue  = np.array([1.00, 1.00, 0.50, 0.40, 0.50],dtype = np.float64)
r.TColor.CreateGradientColorTable(len(stops), stops, red, green, blue, 255)
tdrStyle.SetNumberContours(255)


f_susyxsecs = r.TFile("xsec_susy_13tev_run2.root","READ")
h_susyxsecs = f_susyxsecs.Get("h_xsec_gluino").Clone("h_susyxsecs")

folder_prefix = "limits_T5ZZ_070317_paralleltest"
f_rvalues = r.TFile(os.path.join(folder_prefix,"r-values_T5ZZ.root"),"READ")

massplane_exp = f_rvalues.Get("hExp").Clone("massplane_exp")
massplane_xsec = massplane_exp.Clone("massplane_xsec")
massplane_obs = f_rvalues.Get("hObs").Clone("massplane_obs")
massplane_obs_up = f_rvalues.Get("hObs").Clone("massplane_obs_up")
massplane_obs_dn = f_rvalues.Get("hObs").Clone("massplane_obs_dn")

massplane_exp_up = f_rvalues.Get("hExp1p").Clone("massplane_exp_up")
massplane_exp_up2 = f_rvalues.Get("hExp2p").Clone("massplane_exp_up2")
massplane_exp_dn = f_rvalues.Get("hExp1m").Clone("massplane_exp_dn")
massplane_exp_dn2 = f_rvalues.Get("hExp2m").Clone("massplane_exp_dn2")

contourPlot = massplane_exp.Clone("contourPlot")

h_axis = r.TH2F("h_axis","h_axis",140,1000,2400,240,0,2400)
h_axis.GetXaxis().SetLabelSize(0.035)
h_axis.GetYaxis().SetLabelSize(0.035)
h_axis.GetXaxis().SetRangeUser(1100,2350)
h_axis.GetYaxis().SetRangeUser(100,2350)
h_axis.GetXaxis().SetNdivisions(520)
h_axis.GetXaxis().SetTitle("m_{#tilde{g}} (GeV)")
h_axis.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
h_axis.GetYaxis().SetNdivisions(520)


c_massplane = r.TCanvas("c_massplane","c_massplane",800,800)
c_massplane.cd()
padt = r.TPad("p_tex","p_tex",0.0,0.0,1.0,1.0)
padt.SetTopMargin(0.08)
padt.SetBottomMargin(0.16)
padt.SetRightMargin(0.17)
padt.Draw()
padt.cd()
padt.SetLogz()
h_axis.Draw("axis")

contourPlot.SetContour(1,np.array([1],dtype = np.float64))
contourPlot.SetLineWidth(4)
contourPlot.SetLineStyle(2)
contourPlot.SetLineColor(r.kRed)
contourPlot.Smooth()

massplane_exp_dn.SetContour(1,np.array([1],dtype = np.float64))
massplane_exp_dn.SetLineWidth(2)
massplane_exp_dn.SetLineStyle(2)
massplane_exp_dn.SetLineColor(r.kRed)
massplane_exp_dn.Smooth()

massplane_exp_up.SetContour(1,np.array([1],dtype = np.float64))
massplane_exp_up.SetLineWidth(2)
massplane_exp_up.SetLineStyle(2)
massplane_exp_up.SetLineColor(r.kRed)
massplane_exp_up.Smooth()

massplane_exp_dn2.SetContour(1,np.array([1],dtype = np.float64))
massplane_exp_dn2.SetLineWidth(2)
massplane_exp_dn2.SetLineStyle(3)
massplane_exp_dn2.SetLineColor(r.kRed)
massplane_exp_dn2.Smooth()

massplane_exp_up2.SetContour(1,np.array([1],dtype = np.float64))
massplane_exp_up2.SetLineWidth(2)
massplane_exp_up2.SetLineStyle(3)
massplane_exp_up2.SetLineColor(r.kRed)
massplane_exp_up2.Smooth()

massplane_obs.SetContour(1,np.array([1],dtype = np.float64))
massplane_obs.SetLineWidth(4)
massplane_obs.SetLineStyle(1)
massplane_obs.SetLineColor(r.kBlack)
massplane_obs.Smooth()

massplane_obs_up.SetContour(1,np.array([1],dtype = np.float64))
massplane_obs_up.SetLineWidth(2)
massplane_obs_up.SetLineStyle(2)
massplane_obs_up.SetLineColor(r.kBlack)
massplane_obs_up.Smooth()

massplane_obs_dn.SetContour(1,np.array([1],dtype = np.float64))
massplane_obs_dn.SetLineWidth(2)
massplane_obs_dn.SetLineStyle(2)
massplane_obs_dn.SetLineColor(r.kBlack)
massplane_obs_dn.Smooth()

for binx in range(1,(massplane_xsec.GetNbinsX())+1):
    for biny in range(1,(massplane_xsec.GetNbinsY())+1):
        mG = massplane_xsec.GetXaxis().GetBinCenter(binx)
        trueBin = massplane_xsec.GetBin(binx,biny) #True bin number

        xsec = h_susyxsecs.GetBinContent(h_susyxsecs.FindBin(mG))
        xsec_relErr = h_susyxsecs.GetBinError(h_susyxsecs.FindBin(mG))/xsec

        #Set bin errors and carpet values
        massplane_xsec.SetBinContent(trueBin,massplane_xsec.GetBinContent(trueBin)*xsec)
        massplane_obs_up.SetBinContent(trueBin,massplane_obs.GetBinContent(trueBin) * (1 + xsec_relErr))
        massplane_obs_dn.SetBinContent(trueBin,massplane_obs.GetBinContent(trueBin) * (1-xsec_relErr))

#More contour engineering - to ensure nothing is drawn above the diagonal
for binx in range(1,(massplane_xsec.GetNbinsX())+1):
    for biny in range(1,(massplane_xsec.GetNbinsY())+1):
        mG = massplane_xsec.GetXaxis().GetBinCenter(binx)
        mChi = massplane_xsec.GetYaxis().GetBinCenter(biny)
        trueBin = massplane_xsec.GetBin(binx,biny)
        trueBinMinus1 = massplane_xsec.GetBin(binx,biny-1)

        if mChi in np.arange(mG+1,mG+100):
            penalty = 2
            massplane_obs.SetBinContent(trueBin,massplane_obs.GetBinContent(trueBinMinus1) * penalty)
            massplane_obs_up.SetBinContent(trueBin,massplane_obs_up.GetBinContent(trueBinMinus1) * penalty)
            massplane_obs_dn.SetBinContent(trueBin,massplane_obs_dn.GetBinContent(trueBinMinus1) * penalty)
            massplane_exp_up.SetBinContent(trueBin,massplane_exp_up.GetBinContent(trueBinMinus1) * penalty)
            massplane_exp_dn.SetBinContent(trueBin,massplane_exp_dn.GetBinContent(trueBinMinus1) * penalty)
            massplane_exp_up2.SetBinContent(trueBin,massplane_exp_up2.GetBinContent(trueBinMinus1) * penalty)
            massplane_exp_dn2.SetBinContent(trueBin,massplane_exp_dn2.GetBinContent(trueBinMinus1) * penalty)
            massplane_xsec.SetBinContent(trueBin,massplane_xsec.GetBinContent(trueBinMinus1) * penalty)
            contourPlot.SetBinContent(trueBin,contourPlot.GetBinContent(trueBinMinus1) * penalty)

massplane_xsec.Draw("samecolz")

#Find the bin in massplane_xsec corresponding to 900 and 2400 in mG, and 100 in mLSP

mGLow = massplane_xsec.GetXaxis().FindBin(900)
mGHigh = massplane_xsec.GetXaxis().FindBin(2400)
mLSPLow = massplane_xsec.GetYaxis().FindBin(100)
mLSPHigh = massplane_xsec.GetYaxis().FindBin(2400)
vmG = []
vmLSP = []
vxsec = []
vobs = []
vobsup = []
vobsdown = []
vexp = []
vexpup = []
vexpdown = []

for binx in range(mGLow,mGHigh):
    for biny in range(mLSPLow,mLSPHigh):
        mG = massplane_xsec.GetXaxis().GetBinCenter(binx)
        mLSP = massplane_xsec.GetYaxis().GetBinCenter(biny)
        #hackiest of hacks right here
        if mLSP > mG:
            continue
        print("mG = {},mLSP ={}".format(mG,mLSP))
        trueBin = massplane_xsec.FindBin(mG,mLSP)
        vmG.append(mG)
        vmLSP.append(mLSP)
        vxsec.append(h_susyxsecs.GetBinContent(h_susyxsecs.FindBin(mG)))
        vobs.append(massplane_obs.GetBinContent(trueBin))
        vobsup.append(massplane_obs_up.GetBinContent(trueBin))
        vobsdown.append(massplane_obs_dn.GetBinContent(trueBin))
        vexp.append(massplane_exp.GetBinContent(trueBin))
        vexpup.append(massplane_exp_up.GetBinContent(trueBin))
        vexpdown.append(massplane_exp_dn.GetBinContent(trueBin))

vmG = np.array(vmG,dtype = np.float64)
vmLSP = np.array(vmLSP,dtype = np.float64)
vxsec = np.array(vxsec,dtype = np.float64)
vobs = np.array(vobs,dtype = np.float64)
vobsup = np.array(vobsup,dtype = np.float64)
vobsdown = np.array(vobsdown,dtype = np.float64)
vexp = np.array(vexp,dtype = np.float64)
vexpup = np.array(vexpup,dtype = np.float64)
vexpdown = np.array(vexpdown,dtype = np.float64)

mc_only = False

if mc_only:
    vlim = vxsec * vexp
else:
    vlim = vxsec * vobs

glim = r.TGraph2D("glim","Cross section limit",len(vlim),vmG,vmLSP,vlim)
dots = r.TGraph(len(vmG),vmG,vmLSP)
bin_size = 12.5

glim.SetNpx(max(1,min(600,int(np.ceil((vmG.max() - vmG.min())/bin_size)))))
glim.SetNpy(max(1,min(600,int(np.ceil(vmLSP.max() - vmLSP.min())/bin_size))))

hlim = glim.GetHistogram()
hlim.SetName("hObsXsec")
hlim.GetXaxis().SetLabelSize(0.035)
hlim.GetYaxis().SetLabelSize(0.035)
hlim.GetXaxis().SetTitleSize(0.04)
hlim.GetYaxis().SetTitleSize(0.04)
hlim.GetXaxis().SetTitleOffset(1.25)
hlim.GetYaxis().SetTitleOffset(1.75)
hlim.GetXaxis().SetTitle("m_{#tilde{g}} [GeV]")
hlim.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} [GeV]")
hlim.GetZaxis().SetRangeUser(1e-3,1e-1)
hlim.GetZaxis().SetLabelSize(0)
hlim.GetXaxis().SetRangeUser(1100,2350)
hlim.GetYaxis().SetRangeUser(100,2350)
hlim.GetZaxis().SetTitleSize(0.05)
hlim.GetZaxis().SetTitleOffset(1.25)
hlim.GetZaxis().SetTitle("95 % CL upper limit on #sigma (pb)")
hlim.GetZaxis().SetLabelSize(0.035)


for binx in range(1,(hlim.GetNbinsX())+1):
    for biny in range(1,(hlim.GetNbinsY())+1):
        trueBin = hlim.GetBin(binx,biny)
        mG = hlim.GetXaxis().GetBinCenter(binx)
        mLSP = hlim.GetYaxis().GetBinCenter(biny)
        if mG < 1000 or mG > 2400 or mLSP < 0 or mLSP > mG:
            hlim.SetBinContent(trueBin,0)

hlim.Draw("colz")
contourPlot.Draw("samecont3")
massplane_exp_up.Draw("samecont3")
massplane_exp_dn.Draw("samecont3")
massplane_exp_up2.Draw("samecont3")
massplane_exp_dn2.Draw("samecont3")

if mc_only == False:
    massplane_obs.Draw("samecont3")
    massplane_obs_up.Draw("samecont3")
    massplane_obs_dn.Draw("samecont3")

temp = []
for i in range(0,20):
    temp.append(r.TLine(1100,1100+i*20,2350,2350 + i*20))
    temp[-1].SetLineWidth(10)
    temp[-1].SetLineColor(r.kWhite)
    temp[-1].SetLineStyle(1)
    temp[-1].Draw("same")

padt.RedrawAxis()

box = r.TBox(1100,1850,2350,2350)
box.SetFillColor(r.kWhite)
box.Draw("same")

l1 = r.TLegend(0.205,0.75,0.76,0.83)
l1.SetLineColor(r.kWhite)
l1.SetTextFont(42)
l1.SetTextSize(0.038)
l1.SetShadowColor(r.kWhite)
l1.SetFillColor(r.kWhite)
l1.AddEntry(contourPlot,"Expected limit, #pm 1,2 #sigma_{exp.}", "l")
if not mc_only:
    l1.AddEntry(massplane_obs,"Observed limit, #pm 1 #sigma_{theory}","l" )
l1.Draw("same")

top_margin = r.TLine(1100,2350,2350,2350)
top_margin.SetLineWidth(4)
top_margin.SetLineColor(r.kBlack)
top_margin.SetLineStyle(1)
top_margin.Draw("same")

bot_margin = r.TLine(1100,1850,2350,1850)
bot_margin.SetLineWidth(4)
bot_margin.SetLineColor(r.kBlack)
bot_margin.SetLineStyle(1)
bot_margin.Draw("same")

lef_margin = r.TLine(1100,1850,1100,2350)
lef_margin.SetLineWidth(4)
lef_margin.SetLineColor(r.kBlack)
lef_margin.SetLineStyle(1)
lef_margin.Draw("same")

rig_margin = r.TLine(2350,1850,2350,2350)
rig_margin.SetLineWidth(4)
rig_margin.SetLineColor(r.kBlack)
rig_margin.SetLineStyle(1)
rig_margin.Draw("same")

prctex = r.TLatex(0.215,0.88, "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g}#rightarrow 2j + #tilde{#chi}_{1}^{0}, #tilde{#chi}_{1}^{0} #rightarrow Z#tilde{G}; m_{#tilde{G}} = 1 GeV" )
prctex.SetNDC()
prctex.SetTextSize(0.032)
prctex.SetLineWidth(2)
prctex.SetTextFont(42)
prctex.Draw();

clstex = r.TLatex(0.215,0.84, "NLO + NLL exclusion" )
clstex.SetNDC()
clstex.SetTextSize(0.032)
clstex.SetLineWidth(2)
clstex.SetTextFont(42)
clstex.Draw();


cmstex = r.TLatex(0.575,0.94,"137.2 fb^{-1} (13 TeV)")
cmstex.SetNDC()
cmstex.SetTextSize(0.04)
cmstex.SetLineWidth(2)
cmstex.SetTextFont(42)
cmstex.Draw();

cmstexbold = r.TLatex(0.14,0.94, "CMS" )
cmstexbold.SetNDC()
cmstexbold.SetTextSize(0.05)
cmstexbold.SetLineWidth(2)
cmstexbold.SetTextFont(62)
cmstexbold.Draw()

cmstexPrelim = r.TLatex(0.25,0.94, "Preliminary" )
cmstexPrelim.SetNDC()
cmstexPrelim.SetTextSize(0.0375)
cmstexPrelim.SetLineWidth(2)
cmstexPrelim.SetTextFont(52)
cmstexPrelim.Draw()


c_massplane.SaveAs("/home/users/bsathian/public_html/T5ZZ_Exclusion_13TeV.pdf")
