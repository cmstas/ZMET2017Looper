import ROOT


def findJESDeviation(yeild, JES_up, JES_dn):
  """Returns a list of the greatest deviation in a bin between CV yeild with JES_up and JES_dn"""
  uncs=[]
  for y,up,dn in zip(yeild, JES_up, JES_dn):
    if abs(up-y) > abs(dn-y):
      uncs.append(abs(up-y))
    else:
      uncs.append(abs(dn-y))

  return uncs

def getSignalYields(SR, mass_gluino, mass_lsp, file_path="t5zz.root"):
  """Reads all the histograms for the 2D scan and returns yeilds and uncertainies for the model in the signal region. Returns: (yield, stat uncertainty, btag light unc, btag heavy unc, isr unc, JES unc, fast sim MET unc)"""

  bins=[]

  file_path_JES_up=file_path.replace("/CV/", "/JES_up/")
  file_path_JES_dn=file_path.replace("/CV/", "/JES_dn/")
  #file_path_GenMET=file_path.replace("/CV/", "/GenMET/")

  if (SR=="SRC" or SR=="SRCb"):
    bins = [(50,100), (100,150), (150,6001)]
  elif (SR=="TChiWZ"):
    bins = [(50,100), (100,150), (150,250), (250, 350), (350, 6001)]
  else:
    bins = [(50,100), (100,150), (150,250), (250, 6001)]

  yields = []
  stat_unc_bin = ROOT.Double()
  stat_uncs = []
  bl_yields = []
  bh_yields = []
  isr_yields = []
  JES_up = []
  JES_dn = []

  f=ROOT.TFile(file_path,"r")
  f_JES_up=ROOT.TFile(file_path_JES_up,"r")
  f_JES_dn=ROOT.TFile(file_path_JES_dn,"r")

  count=f.Get("susy_type1MET_counts").Clone("count")
  btag_light_up=f.Get("susy_type1MET_btaglight_up").Clone("btag_light_up")
  btag_heavy_up=f.Get("susy_type1MET_btagheavy_up").Clone("btag_heavy_up")
  isr_up=f.Get("susy_type1MET_isr_up").Clone("isr_up")
  JES_up_count=f_JES_up.Get("susy_type1MET_counts").Clone("JES_up_count")
  JES_dn_count=f_JES_dn.Get("susy_type1MET_counts").Clone("JES_dn_count")

  for b in bins:
    yields.append(count.IntegralAndError(count.GetXaxis().FindBin(b[0]), 
                                         count.GetXaxis().FindBin(b[1] - 0.001), 
                                         count.GetYaxis().FindBin(mass_gluino), 
                                         count.GetYaxis().FindBin(mass_gluino), 
                                         count.GetZaxis().FindBin(mass_lsp), 
                                         count.GetZaxis().FindBin(mass_lsp), 
                                         stat_unc_bin))
    stat_uncs.append(stat_unc_bin)
    
    bl_yields.append(btag_light_up.Integral(btag_light_up.GetXaxis().FindBin(b[0]),
                                            btag_light_up.GetXaxis().FindBin(b[1] - 0.001), 
                                            btag_light_up.GetYaxis().FindBin(mass_gluino), 
                                            btag_light_up.GetYaxis().FindBin(mass_gluino), 
                                            btag_light_up.GetZaxis().FindBin(mass_lsp), 
                                            btag_light_up.GetZaxis().FindBin(mass_lsp)))

    bh_yields.append(btag_heavy_up.Integral(btag_heavy_up.GetXaxis().FindBin(b[0]), 
                                            btag_heavy_up.GetXaxis().FindBin(b[1] - 0.001), 
                                            btag_heavy_up.GetYaxis().FindBin(mass_gluino), 
                                            btag_heavy_up.GetYaxis().FindBin(mass_gluino), 
                                            btag_heavy_up.GetZaxis().FindBin(mass_lsp), 
                                            btag_heavy_up.GetZaxis().FindBin(mass_lsp)))

    isr_yields.append(isr_up.Integral(isr_up.GetXaxis().FindBin(b[0]), 
                                      isr_up.GetXaxis().FindBin(b[1] - 0.001), 
                                      isr_up.GetYaxis().FindBin(mass_gluino), 
                                      isr_up.GetYaxis().FindBin(mass_gluino), 
                                      isr_up.GetZaxis().FindBin(mass_lsp), 
                                      isr_up.GetZaxis().FindBin(mass_lsp)))

    JES_up.append(JES_up_count.Integral(JES_up_count.GetXaxis().FindBin(b[0]), 
                                         JES_up_count.GetXaxis().FindBin(b[1] - 0.001), 
                                         JES_up_count.GetYaxis().FindBin(mass_gluino), 
                                         JES_up_count.GetYaxis().FindBin(mass_gluino), 
                                         JES_up_count.GetZaxis().FindBin(mass_lsp), 
                                         JES_up_count.GetZaxis().FindBin(mass_lsp)))

    JES_dn.append(JES_dn_count.Integral(JES_dn_count.GetXaxis().FindBin(b[0]), 
                                         JES_dn_count.GetXaxis().FindBin(b[1] - 0.001), 
                                         JES_dn_count.GetYaxis().FindBin(mass_gluino), 
                                         JES_dn_count.GetYaxis().FindBin(mass_gluino), 
                                         JES_dn_count.GetZaxis().FindBin(mass_lsp), 
                                         JES_dn_count.GetZaxis().FindBin(mass_lsp)))




  return (yields, stat_uncs, bl_yields, bh_yields, isr_yields, findJESDeviation(yeilds, JES_up, JES_dn))

