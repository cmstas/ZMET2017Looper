#!/usr/bin/env python

import argparse, sys, ROOT

basedir="/nfs-7/userdata/bobak/ZMET2017_Hists/SignalCutFlows/"
lumi_fb=35.9
Xsecs = {}

def getXSec(model, mass_point):
  """Loads the proper TChain for the model and loops through events until it finds the mass point given and returns evt_xsec, if the mass point can not be found, it returns None"""
  m_glu=None
  m_lsp=None
  m_chi=None
  global Xsecs

  ch = ROOT.TChain("t")

  if model == "t5zz":
    m_glu = mass_point[0]
    m_lsp = mass_point[1]
    
    if (("%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)) in Xsecs.keys()):
      return Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)]
    
    if m_glu >= 1850:
      ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
    else:
      ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")
  elif model == "tchiwz":
    m_glu = mass_point[0]
    m_lsp = mass_point[1]
    if (("%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)) in Xsecs.keys()):
      return Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/tchiwz_80x_v2*")
  elif model == "tchihz":
    m_chi = mass_point
    if (("%s_mchi%d" % (model, m_chi)) in Xsecs.keys()):
      return Xsecs["%s_mchi%d" % (model, m_chi)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchihz_80x_v2*")
  elif model == "tchizz":
    m_chi = mass_point
    if (("%s_mchi%d" % (model, m_chi)) in Xsecs.keys()):
      return Xsecs["%s_mchi%d" % (model, m_chi)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchizz_80x_v2*")

  if m_glu:
    ch.SetBranchStatus("*", 0)
    ch.SetBranchStatus("mass_LSP", 1)
    ch.SetBranchStatus("mass_gluino", 1)
    ch.SetBranchStatus("evt_xsec", 1)

    n_entries = ch.GetEntries()
    for j_entry in range(n_entries):

      i_entry = ch.LoadTree(j_entry)
      if i_entry < 0:
        break
      nb = ch.GetEntry(j_entry)
      if nb <= 0:
        continue


      if ((ch.mass_gluino == m_glu) and (ch.mass_LSP == m_lsp)):
        Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)] = ch.evt_xsec
        return ch.evt_xsec
  
  elif m_chi:
    ch.SetBranchStatus("*", 0)
    ch.SetBranchStatus("mass_chi", 1)
    ch.SetBranchStatus("evt_xsec", 1)

    n_entries = ch.GetEntries()
    for j_entry in range(n_entries):

      i_entry = ch.LoadTree(j_entry)
      if i_entry < 0:
        break
      nb = ch.GetEntry(j_entry)
      if nb <= 0:
        continue


      if (ch.mass_chi == m_chi):
        Xsecs["%s_mchi%d" % (model, m_chi)] = ch.evt_xsec
        return ch.evt_xsec

  return None

def makeT5ZZCutFlows(m_glu, m_lsp):
  print("SRA")
  print("T5ZZ model, mass gluino: %.0f, mass LSP: %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeSRATable(m_glu, m_lsp)
  
  print("SRB")
  print("T5ZZ model, mass gluino: %.0f, mass LSP: %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeSRBTable(m_glu, m_lsp)
  
  print("SRC")
  print("T5ZZ model, mass gluino: %.0f, mass LSP: %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeSRCTable(m_glu, m_lsp)

def makeTChiWZCutFlows(m_glu, m_lsp):
  print("TChiWZ")
  print("TChiWZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
  n = getXSec("tchiwz", [m_glu, m_lsp])
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeTChiWZTable(m_glu, m_lsp, "tchiwz")
  
  print("TChiHZ")
  print("TChiWZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
  n = getXSec("tchiwz", [m_glu, m_lsp])
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeTChiHZTable(m_glu, m_lsp, "tchiwz")
  
def makeTChiZZCutFlows(m_chi):
  print("TChiWZ")
  print("TChiZZ model, mass chi: %.0f || Events in 35.9 fb$^{-1}$" % (m_chi))
  n = getXSec("tchizz", m_chi)
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeTChiWZTable(-1,-1,"tchizz", m_chi)
  
  print("TChiHZ")
  print("TChiZZ model, mass chi: %.0f || Events in 35.9 fb$^{-1}$" % (m_chi))
  n = getXSec("tchizz", m_chi)
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeTChiHZTable(-1,-1,"tchizz", m_chi)

def makeTChiHZCutFlows(m_chi):
  print("TChiHZ")
  print("TChiHZ model, mass chi: %.0f || Events in 35.9 fb$^{-1}$" % (m_chi))
  n = getXSec("tchihz", m_chi)
  print("All Entries || %f" % (n*1000*lumi_fb))
  makeTChiHZTable(-1,-1,"tchihz", m_chi)

def makeSRATable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRA/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2-3 Jets|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("Btag requirement || B Veto || >= 1 Btag")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  # ==========================
  # MT2 Start
  # ==========================
  print("MT2 > || 80 GeV || 100 GeV")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  # ==========================
  # HT and MET Start
  # ==========================
  print("$H_{T}$ > || 500 GeV || 200 GeV")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  bveto_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
  print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeSRBTable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRB/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("4-5 Jets|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("Btag requirement || B Veto || >= 1 Btag")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  # ==========================
  # MT2 Start
  # ==========================
  print("MT2 > || 80 GeV || 100 GeV")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  # ==========================
  # HT and MET Start
  # ==========================
  print("$H_{T}$ > || 500 GeV || 200 GeV")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  bveto_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
  print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeSRCTable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRC/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("6+ Jets|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("Btag requirement || B Veto || >= 1 Btag")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))

  # ==========================
  # MT2 and MET Start
  # ==========================
  print("MT2 > || 80 GeV || 100 GeV")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  bveto_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print(" || %f || %f" % (n_bveto, n_btag))


  print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
  print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeTChiHZTable(m_glu, m_lsp, model, m_chi=None):
  if model == "tchiwz":
    hists_path = basedir+"TChiWZ/TChiHZ/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  elif model == "tchihz":
    hists_path = basedir+"TChiHZ/TChiHZ/mchi%d_" % (m_chi)
  elif model == "tchizz":
    hists_path = basedir+"TChiZZ/TChiHZ/mchi%d_" % (m_chi)
  else:
    print ("No proper model given for TChiHZ table. Got: %s" % model)
    return None

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2 leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("6+ Jets|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Exactly 2 btags || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2b.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2b")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("MT2b > 200 GeV || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2b_Mbb.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2b_Mbb")
  n=h_met.Integral(1,6001)
  met_counts=[]
  met_counts.append(h_met.Integral(100,6001))
  met_counts.append(h_met.Integral(150,6001))
  met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print("M$_{bb} <$  150 GeV|| %f" %n)

  print("$E^{miss}_{T} > 100$ GeV || %f" % met_counts[0])
  print("$E^{miss}_{T} > 150$ GeV || %f" % met_counts[1])
  print("$E^{miss}_{T} > 250$ GeV || %f" % met_counts[2])

def makeTChiWZTable(m_glu, m_lsp, model, m_chi=None):
  if model == "tchiwz":
    hists_path = basedir+"TChiWZ/TChiWZ/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  elif model == "tchizz":
    hists_path = basedir+"TChiZZ/TChiWZ/mchi%d_" % (m_chi)
  else:
    print ("No proper model given for TChiWZ table. Got: %s" % model)
    return None

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2 leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("6+ Jets|| %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("No btags || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("MT2 > 80 GeV || %f" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_Mjj.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_Mjj")
  n=h_met.Integral(1,6001)
  met_counts=[]
  met_counts.append(h_met.Integral(100,6001))
  met_counts.append(h_met.Integral(150,6001))
  met_counts.append(h_met.Integral(250,6001))
  met_counts.append(h_met.Integral(350,6001))
  f_met.Close()
  print("M$_{jj}$ for min $\Delta \Phi$ jets < 150 GeV|| %f" %n)

  print("$E^{miss}_{T} > 100$ GeV || %f" % met_counts[0])
  print("$E^{miss}_{T} > 150$ GeV || %f" % met_counts[1])
  print("$E^{miss}_{T} > 250$ GeV || %f" % met_counts[2])
  print("$E^{miss}_{T} > 350$ GeV || %f" % met_counts[3])

if __name__ == "__main__":
  #makeT5ZZCutFlows(1400,700)
  #makeTChiWZCutFlows(550,200)
  #makeTChiZZCutFlows(350)
  #makeTChiHZCutFlows(350)
  parser = argparse.ArgumentParser()

  parser.add_argument("--t5zz", help="print cutflow table for T5ZZ sample (must specify mass_gluino and mass_lsp)", action="store_true")
  parser.add_argument("--tchiwz", help="print cutflow table for TChiWZ sample (must specify mass_gluino and mass_lsp)", action="store_true")
  parser.add_argument("--tchizz", help="print cutflow table for TChiZZ sample (must specify mass_chi)", action="store_true")
  parser.add_argument("--tchihz", help="print cutflow table for TChiHZ sample (must specify mass_chi)", action="store_true")
  parser.add_argument("--mass_chi", help="choose chi mass for TChiHZ or TChiZZ sample.", type=int)
  parser.add_argument("--mass_gluino", help="choose gluino mass for T5ZZ or TChiWZ sample.", type=int)
  parser.add_argument("--mass_lsp", help="choose chi mass for T5ZZ or TChiWZ sample.", type=int)
  
  args=parser.parse_args()

  if (args.t5zz):
    if args.mass_gluino and args.mass_lsp:
      makeT5ZZCutFlows(args.mass_gluino, args.mass_lsp)
    else:
      print("You must specify a gluino and lsp mass to make that table")
  elif (args.tchiwz):
    if args.mass_gluino and args.mass_lsp:
      makeTChiWZCutFlows(args.mass_gluino, args.mass_lsp)
    else:
      print("You must specify a gluino and lsp mass to make that table")
  elif (args.tchizz):
    if args.mass_chi:
      makeTChiZZCutFlows(args.mass_chi)
    else:
      print("You must specify a chi mass to make that table")
  elif (args.tchihz):
    if args.mass_chi:
      makeTChiHZCutFlows(args.mass_chi)
    else:
      print("You must specify a chi mass to make that table")
  else:
    parser.print_help()
    exit()

