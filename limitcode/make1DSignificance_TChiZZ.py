from __future__ import print_function
import ROOT, array
import sys, glob, math
ROOT.gROOT.SetBatch(True)
#lumi = 35.9
lumi = 137.2

fxsec = None
hxsec = None

def get1Dlimit(fn):
    print("file : ",fn)
    f = ROOT.TFile.Open(fn)
    tree= f.Get("limit")
    limits = []
    for entry in tree:
        limit = entry.limit
    return limit


def main():
    version = "significance_TChiZZ_070317_paralleltest"
    dir="./"+version+"/"
#    chargino_masses =[100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750]
#    chargino_masses =[100,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700]
    chargino_masses =[100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950]
    #chargino_masses =[125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000,1025,1050,1075,1100,1125,1150,1175,1200]

    obs=[]

    h_obs = ROOT.TH1F("hObs","",37, 87.5, 1012.5)
    h_obs.GetXaxis().SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]")
    h_obs.GetYaxis().SetTitle("Observed significance")

    for i,chargino in enumerate(chargino_masses):
        fn = dir+'limit_TChiZZ_'+str(chargino)+'.root'
        limit = get1Dlimit(fn)
        obs.append(limit)
        h_obs.SetBinContent(i+1,obs[i])

    chi1n = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
    chi2n = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.]{#scale[0.85]{_{2}}}"
    chi1pm= "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
    chii= "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#pm}}}#kern[-3.]{#scale[0.85]{_{i}}}"
    chij= "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#mp}}}#kern[-3.]{#scale[0.85]{_{j}}}"
    chi10= "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
    xsoft= "X#scale[0.85]{_{soft}}"
    mass_ = "m#kern[0.1]{#lower[-0.12]{_{"
    ppChiChi = "pp #rightarrow "+chii+"#kern[0.6]{"+chij+"}  #rightarrow "+chi10+"#kern[0.3]{"+chi10+"} + "+xsoft+"#rightarrow ZZ#tilde{G}#tilde{G} + "+xsoft
    branching = "BR("+chi10+"#rightarrow Z#tilde{G}) = 100%"
    mChis = mass_+chi2n+"}}} #approx "+mass_+chi1pm+"}}} #approx "+mass_+chi1n+"}}}, "+mass_+"#tilde{G}}}} = 1 GeV"

    ROOT.gStyle.SetOptStat(0)
    c1 = ROOT.TCanvas("c1", "", 800, 800)
    c1.cd()
    padt = ROOT.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0)
    padt.SetTopMargin(0.1)
    padt.SetBottomMargin(0.1)
    padt.SetRightMargin(0.05)
    padt.SetLeftMargin(0.15)
    padt.SetTickx()
    padt.SetTicky()
    padt.Draw()
    padt.cd()
    h = ROOT.TH2F("h","h",68,100,950,100,-3,3)
    h.SetTitle("")
    h.GetXaxis().SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]")
    h.GetYaxis().SetTitle("#sigma limit at 95% CL [pb]")
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetTitleSize(0.042)
    h.GetYaxis().SetTitleSize(0.042)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)
    h.Draw()

    print(obs)
    gobs = ROOT.TGraph(len(chargino_masses), array.array('d', chargino_masses), array.array('d', obs))
    gobs.SetMarkerStyle(ROOT.kFullCircle)
    gobs.SetMarkerSize(1.5)
    gobs.SetMarkerColor(ROOT.kBlue)
    gobs.SetLineWidth(3)
    gobs.SetLineColor(ROOT.kBlue)
    gobs.Draw("L")

    prctex = ROOT.TLatex(0.25,0.83, ppChiChi );
    prctex.SetNDC()
    prctex.SetTextSize(0.032)
    prctex.SetLineWidth(2)
    prctex.SetTextFont(42)
    prctex.Draw()

    prctex2 = ROOT.TLatex(0.25,0.78, branching );
    prctex2.SetNDC()
    prctex2.SetTextSize(0.032)
    prctex2.SetLineWidth(2)
    prctex2.SetTextFont(42)
    prctex2.Draw()

    prctex3 = ROOT.TLatex(0.25,0.73, mChis );
    prctex3.SetNDC()
    prctex3.SetTextSize(0.032)
    prctex3.SetLineWidth(2)
    prctex3.SetTextFont(42)
    prctex3.Draw()

    cmstex = ROOT.TLatex(0.675,0.91, "%.1f fb^{-1} (13 TeV)" % lumi)
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstex.Draw()

    cmstexbold = ROOT.TLatex(0.17,0.91, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexbold.Draw()

    cmstexprel = ROOT.TLatex(0.29,0.91, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.03)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    l1 = ROOT.TLegend(0.45, 0.52, 0.9, 0.71)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetLineColor(ROOT.kWhite)
    l1.SetShadowColor(ROOT.kWhite)
    l1.SetFillColor(ROOT.kWhite)
    l1.AddEntry(gobs , "Observed Significance", "l")
    l1.Draw()
    c1.SaveAs("~/public_html/TChiZZ_Significance_13TeV.pdf")

#####################################################################################################
if __name__ == "__main__":
    main()
