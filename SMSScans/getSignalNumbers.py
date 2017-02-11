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
  f=ROOT.TFile(file_path,"r")
  met=f.Get("susy_type1MET_counts").Clone("met")

  for b in bins:
    yields.append(met.Integral(met.FindBin(b[0]), met.FindBin(b[1] - 0.001), mass_gluino, mass_gluino, mass_lsp, mass_lsp))

  return yields

