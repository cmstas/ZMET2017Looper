import ROOT,sys,os

def getPercentBelowDR(SR, m1, m2, DR):
  path = "/nfs-7/userdata/bobak/ZMET2017_Hists/FatJetStudy/"
  if SR == "wz":
    path+="TChiWZ/Vanilla/tchiwz_mglu%d_mlsp%d.root" % (m1,m2)
  elif SR == "wz":
    path+="TChiHZ/Vanilla/tchihz_mchi%d.root" % m1
  elif SR == "zz":
    path+="TChiZZ/Vanilla/tchizz_mchi%d.root" % m1
  else:
    raise NameError("Signal Region %s unknown" % SR)

  f = ROOT.TFile(path)
  h = f.Get("fj_DeltaR").Clone("h_fj_DR")

  return h.Integral(h.FindBin(0),h.FindBin(DR))/h.Integral()

def main():
  mp_tchiwz = [(400,175), (500,250), (600,275), (650,50), (700,50)]
  mp_tchihz = [(400,-1), (500,-1), (600,-1), (700,-1)]
  mp_tchizz = [(600,-1), (700,-1), (800,-1)]

  print("TChiWZ-------------------------")
  for mp in mp_tchiwz:
    print("(%d, %d) BosonPt DeltaR (%f\% below DR = 1, %f\% below DR = 0.4)" % (mp[0],mp[1], getPercentBelowDR("tchiwz",mp[0],mp[1],1), getPercentBelowDR("tchiwz",mp[0],mp[1],0.4)) )
  print("TChiHZ-------------------------")
  for mp in mp_tchihz:
    print("(%d) BosonPt DeltaR (%f\% below DR = 1, %f\% below DR = 0.4)" % (mp[0],mp[1], getPercentBelowDR("tchihz",mp[0],mp[1],1), getPercentBelowDR("tchihz",mp[0],mp[1],0.4)) )
  print("TChiZZ-------------------------")
  for mp in mp_tchizz:
    print("(%d) BosonPt DeltaR (%f\% below DR = 1, %f\% below DR = 0.4)" % (mp[0],mp[1], getPercentBelowDR("tchihz",mp[0],mp[1],1), getPercentBelowDR("tchihz",mp[0],mp[1],0.4)) )



if __name__ == "__main__":
  main()