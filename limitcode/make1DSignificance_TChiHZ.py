from __future__ import print_function
import ROOT, array
import sys, glob, math
ROOT.gROOT.SetBatch(True)
lumi = 137.2
branching_ratio = 1.0 # already accounted for in datacards
#branching_ratio = 0.5

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
#    version = "limits_TChiHZ_230317"
    version = "significance_TChiHZ_070317_paralleltest"
#    version = "limits_TChiHZ_withzz_310317"
    dir="./"+version+"/"
#    chargino_masses =[100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750]
#    chargino_masses =[100,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700]
    chargino_masses =[127,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950]
    f_xsecgraph = ROOT.TFile.Open("xsec_susy_13tev_final.root")
    h_xsec_c1n2 = f_xsecgraph.Get("h_xsec_higgsino")
    g_xsec_c1n2 = ROOT.TGraph(h_xsec_c1n2)

    obs=[]

    h_obs = ROOT.TH1F("hObs","",36, 112.5, 1012.5)

    for i,chargino in enumerate(chargino_masses):
        fn = dir+'limit_TChiHZ_'+str(chargino)+'.root'
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
    ppChiChi = "pp #rightarrow "+chii+"#kern[0.6]{"+chij+"}  #rightarrow "+chi10+"#kern[0.3]{"+chi10+"} + "+xsoft+"#rightarrow hZ#tilde{G}#tilde{G} + "+xsoft
    branching = "BR("+chi10+"#rightarrow h#tilde{G}) = BR("+chi10+"#rightarrow Z#tilde{G}) = 50%"
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
    h.GetYaxis().SetTitle("Observed significance")
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetTitleSize(0.042)
    h.GetYaxis().SetTitleSize(0.042)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)
    h.Draw()

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
#    l1.SetHeader("#sigma exclusion limit, M_{#tilde{#chi}^{0}_{1}} = 1 GeV")
    # l1.AddEntry(gobs , "Observed 2016", "lp")
    # l1.AddEntry(gexp , "Expected 2016", "l")
    # l1.AddEntry(gexp2x , "Expected 2x lumi", "l")
    # l1.AddEntry(gexp3x , "Expected 3x lumi", "l")
    l1.AddEntry(gobs , "Observed Significance", "l")
    l1.Draw()
    '''
    LExp1 = ROOT.TGraphAsymmErrors(2)
    LExp1.SetFillColor(ROOT.kYellow)
    LExp1.SetPoint(0,250+100000,100000)
    LExp1.SetPointError(0,0.,0.,0.15,0.15)
    LExp1.SetPoint(1,100000, 100000)
    LExp1.SetPointError(1,0.,0.,0.15,0.15)
    LExp1.Draw("3")

    LExp2 = ROOT.TGraphAsymmErrors(2)
    LExp2.SetFillColor(ROOT.kGreen)
    LExp2.SetPoint(0,100000,100000)
    LExp2.SetPointError(0,0.,0.,0.08,0.08)
    LExp2.SetPoint(1,100000,100000)
    LExp2.SetPointError(1,0.,0.,0.08,0.08)
    LExp2.Draw("L3")

    LExp = ROOT.TGraph(2)
    LExp.SetLineColor(ROOT.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,250+ 3.8*(1050-250)/100, 5-2.08*(5-0)/100*10)
    LExp.SetPoint(1,250+21.2*(1050-250)/100, 5-2.08*(5-0)/100*10)
    LExp.Draw("L")
    '''
   #masstex = ROOT.TLatex(0.70,0.79, "H_{T} > 300 GeV" )
    #masstex.SetNDC()
    #masstex.SetTextSize(0.036)
    #masstex.SetLineWidth(2)
    #masstex.SetTextFont(42)
    #masstex.Draw()
    c1.SaveAs("~/public_html/TChiHZ_0p25ZZ_Significance_13TeV.pdf")

    ### store histogram / TGraph versions of limits

#####################################################################################################
if __name__ == "__main__":
    main()
