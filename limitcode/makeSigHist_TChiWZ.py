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



folder_prefix = "significance_TChiWZ_070317_paralleltest"
f_rvalues = r.TFile(os.path.join(folder_prefix,"significances_TChiWZ.root"),"READ")

massplane_obs = f_rvalues.Get("hObs").Clone("massplane_obs")



h_axis = r.TH2F("h_axis","h_axis",80,100,925,55,0,550)
h_axis.GetXaxis().SetLabelSize(0.035)
h_axis.GetYaxis().SetLabelSize(0.035)
h_axis.GetXaxis().SetRangeUser(100,900)
h_axis.GetYaxis().SetRangeUser(0,400)
#h_axis.GetXaxis().SetNdivisions(520)
h_axis.GetXaxis().SetTitle("m_{#tilde{#chi}^{#pm}_{1}} =m_{#tilde{#chi}^{0}_{2}}(GeV)")
h_axis.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
#h_axis.GetYaxis().SetNdivisions(520)


c_massplane = r.TCanvas("c_massplane","c_massplane",800,800)
c_massplane.cd()
padt = r.TPad("p_tex","p_tex",0.0,0.0,1.0,1.0)
padt.SetTopMargin(0.08)
padt.SetBottomMargin(0.16)
padt.SetRightMargin(0.17)
padt.Draw()
padt.cd()
#padt.SetLogz()
h_axis.Draw("axis")


#Find the bin in massplane_xsec corresponding to 900 and 2400 in mG, and 100 in mLSP



#massplane_obs.Draw("samecolz")

mGLow = massplane_obs.GetXaxis().FindBin(100)
mGHigh = massplane_obs.GetXaxis().FindBin(925)
mLSPLow = massplane_obs.GetYaxis().FindBin(0)
mLSPHigh = massplane_obs.GetYaxis().FindBin(550)
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
        mG = massplane_obs.GetXaxis().GetBinCenter(binx)
        mLSP = massplane_obs.GetYaxis().GetBinCenter(biny)
        #hackiest of hacks right here
        trueBin = massplane_obs.FindBin(mG,mLSP)
        if massplane_obs.GetBinContent(trueBin) == 0:
            continue
        print("mG = {},mLSP ={}".format(mG,mLSP))
        trueBin = massplane_obs.FindBin(mG,mLSP)
        vmG.append(mG)
        vmLSP.append(mLSP)
        vobs.append(massplane_obs.GetBinContent(trueBin))

vobs = np.array(vobs,dtype = np.float64)
vmG = np.array(vmG,dtype = np.float64)
vmLSP = np.array(vmLSP,dtype = np.float64)



glim = r.TGraph2D("glim","Cross section limit",len(vobs),vmG,vmLSP,vobs)
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
hlim.GetXaxis().SetTitle("m_{#tilde{#chi}^{#pm}_{1}} =m_{#tilde{#chi}^{0}_{2}}(GeV)")
hlim.GetYaxis().SetTitle("m_{#tilde{#chi}_{1}^{0}} (GeV)")
hlim.GetZaxis().SetLabelSize(0)
hlim.GetXaxis().SetRangeUser(100,900)
hlim.GetYaxis().SetRangeUser(0,550)
hlim.GetZaxis().SetTitleSize(0.05)
hlim.GetZaxis().SetTitleOffset(1.25)
hlim.GetZaxis().SetTitle("Observed Significance")
hlim.GetZaxis().SetLabelSize(0.035)


for binx in range(1,(hlim.GetNbinsX())+1):
    for biny in range(1,(hlim.GetNbinsY())+1):
        trueBin = hlim.GetBin(binx,biny)
        mG = hlim.GetXaxis().GetBinCenter(binx)
        mLSP = hlim.GetYaxis().GetBinCenter(biny)
        if mG < 100 or mG > 925 or mLSP < 0 or mG - mLSP < 90:
            hlim.SetBinContent(trueBin,0)

hlim.Draw("colz")

temp = []
for i in range(0,80):
    temp.append(r.TLine(100,5+i*5,900,810 + i*5))
    temp[-1].SetLineWidth(10)
    temp[-1].SetLineColor(r.kWhite)
    temp[-1].SetLineStyle(1)
    temp[-1].Draw("same")

padt.RedrawAxis()

box = r.TBox(100,420,900,550)
box.SetFillColor(r.kWhite)
box.Draw("same")

l1 = r.TLegend(0.205,0.75,0.76,0.83)
l1.SetLineColor(r.kWhite)
l1.SetTextFont(42)
l1.SetTextSize(0.038)
l1.SetShadowColor(r.kWhite)
l1.SetFillColor(r.kWhite)
l1.Draw("same")

top_margin = r.TLine(100,550,900,550)
top_margin.SetLineWidth(4)
top_margin.SetLineColor(r.kBlack)
top_margin.SetLineStyle(1)
top_margin.Draw("same")

bot_margin = r.TLine(100,420,900,420)
bot_margin.SetLineWidth(4)
bot_margin.SetLineColor(r.kBlack)
bot_margin.SetLineStyle(1)
bot_margin.Draw("same")

lef_margin = r.TLine(100,420,100,550)
lef_margin.SetLineWidth(4)
lef_margin.SetLineColor(r.kBlack)
lef_margin.SetLineStyle(1)
lef_margin.Draw("same")

rig_margin = r.TLine(900,420,900,550)
rig_margin.SetLineWidth(4)
rig_margin.SetLineColor(r.kBlack)
rig_margin.SetLineStyle(1)
rig_margin.Draw("same")

prctex = r.TLatex(0.215,0.88, "pp #rightarrow #tilde{#chi}^{#pm}_{1} #tilde{#chi}^{0}_{2}; #tilde{#chi}^{0}_{2}#rightarrow z + #tilde{#chi}_{1}^{0}, #tilde{#chi}^{#pm}_{1}#rightarrow w^{#pm} + #tilde{#chi}_{1}^{0} ")
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


c_massplane.SaveAs("/home/users/bsathian/public_html/TChiWZ_Significance_13TeV.pdf")

