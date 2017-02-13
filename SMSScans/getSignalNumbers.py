import ROOT

def getSignalYields(SR, mass_gluino, mass_lsp, file_path="t5zz.root"):

  bins=[]

  if (SR=="SRC" or SR=="SRCb"):
    bins = [(50,100), (100,150), (150,6001)]
  elif (SR=="TChiWZ"):
    bins = [(50,100), (100,150), (150,250), (250, 350), (350, 6001)]
  else:
    bins = [(50,100), (100,150), (150,250), (250, 6001)]

  yields = []
  bl_yields = []
  bh_yields = []
  isr_yields = []

  f=ROOT.TFile(file_path,"r")
  count=f.Get("susy_type1MET_counts").Clone("count")
  btag_light_up=f.Get("susy_type1MET_btaglight_up").Clone("btag_light_up")
  btag_heavy_up=f.Get("susy_type1MET_btagheavy_up").Clone("btag_heavy_up")
  isr_up=f.Get("susy_type1MET_isr_up").Clone("isr_up")

  for b in bins:
    yields.append(count.Integral(count.GetXaxis().FindBin(b[0]), count.GetXaxis().FindBin(b[1] - 0.001), count.GetYaxis().FindBin(mass_gluino), count.GetYaxis().FindBin(mass_gluino), count.GetZaxis().FindBin(mass_lsp), count.GetZaxis().FindBin(mass_lsp)))
    
    bl_yields.append(btag_light_up.Integral(btag_light_up.GetXaxis().FindBin(b[0]), btag_light_up.GetXaxis().FindBin(b[1] - 0.001), btag_light_up.GetYaxis().FindBin(mass_gluino), btag_light_up.GetYaxis().FindBin(mass_gluino), btag_light_up.GetZaxis().FindBin(mass_lsp), btag_light_up.GetZaxis().FindBin(mass_lsp)))
    bh_yields.append(btag_heavy_up.Integral(btag_heavy_up.GetXaxis().FindBin(b[0]), btag_heavy_up.GetXaxis().FindBin(b[1] - 0.001), btag_heavy_up.GetYaxis().FindBin(mass_gluino), btag_heavy_up.GetYaxis().FindBin(mass_gluino), btag_heavy_up.GetZaxis().FindBin(mass_lsp), btag_heavy_up.GetZaxis().FindBin(mass_lsp)))

    isr_yields.append(isr_up.Integral(isr_up.GetXaxis().FindBin(b[0]), isr_up.GetXaxis().FindBin(b[1] - 0.001), isr_up.GetYaxis().FindBin(mass_gluino), isr_up.GetYaxis().FindBin(mass_gluino), isr_up.GetZaxis().FindBin(mass_lsp), isr_up.GetZaxis().FindBin(mass_lsp)))

  return (yields, bl_yields, bh_yields, isr_yields)

