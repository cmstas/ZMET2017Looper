#!/usr/bin/env python

import argparse, sys, re, getSignalNumbers

templates_path="SMSScans/Templates/"
signal_name = None
mass_spectrum = None
histogram_Path = None
output_path = None
n_parms = {}

def properSpacing(key, param):
  """return param padded with spaces so that it's length {key} to preserve tabbing in templates"""

  delta=len(key)-len(param)

  if (len(param) < len(key)):
    param=(" "*delta)+param

  return param

def addSignalYields(d, SR, mass_1, mass_2, BR_key=None):
  """Pulls and computes CV. yields, stat uncertainty, btag light SF unc, btag heavy SF unc, and ISR SF unc from the signal scan histogram at the proper mass point.
  mass 1 is meant for the gluino or chi and mass_2 is meant for the LSP, send mass_2 = -1 for 1D scan"""

  if BR_key:
    file_name="%s_%s" %(signal_name, BR_key)
  elif (signal_name == "tchiwz_ext"):
    file_name="tchiwz"
  else:
    file_name=signal_name

  avg_yields, RecoMET_yields, stat_uncs, bl_yields, bh_yields, isr_yields, JES = getSignalNumbers.getSignalYields(SR, mass_1, mass_2, "%s/%s.root" % (histogram_Path, file_name))

  for i,y in enumerate(RecoMET_yields):
    stat_nuisence = 0
    bl_nuisence = 0
    bh_nuisence = 0
    isr_nuisence = 0
    JES_nuisence = 0
    avg_y = avg_yields[i]
    met_nuisence = 0

    #make sure we don't divide by 0
    if y != 0:
      stat_nuisence = (1 + stat_uncs[i]/float(y))
      bl_nuisence = bl_yields[i]/float(y)
      bh_nuisence = bh_yields[i]/float(y)
      isr_nuisence = isr_yields[i]/float(y)
      JES_nuisence = 1+(JES[i]/float(y))
    if avg_y != 0:
      met_nuisence = 1+(abs(y-avg_y)/float(avg_y))

    d["BGbin%d_sig" % i] = properSpacing("{BGbin1_sig}", "%.3f" % avg_y)
    d["sig_stat_syst_bin%d" % i] = properSpacing("{sig_stat_syst_bin1}","%.3f" % stat_nuisence)

    d["sig_btaglight_syst_bin%d" % i] = properSpacing("{sig_btaglight_syst_bin1}", "%.3f" % bl_nuisence)
    d["sig_btagheavy_syst_bin%d" % i] = properSpacing("{sig_btagheavy_syst_bin1}", "%.3f" % bh_nuisence)

    d["sig_isr_syst_bin%d" % i] = properSpacing("{sig_isr_syst_bin1}", "%.4f" % isr_nuisence)

    d["sig_JES_syst_bin%d" % i] = properSpacing("{sig_JES_syst_bin1}","%.3f" % JES_nuisence)

    d["sig_metfromFS_syst_bin%d" % i] = properSpacing("{sig_metfromFS_syst_bin1}","%.3f" % met_nuisence)

def addConstantVals(d):
  d["sig_trig_syst"] = properSpacing("{sig_trig_syst}","1.03")
  d["sig_leptonFS_syst"] = properSpacing("{sig_leptonFS_syst}","1.04")
  d["sig_leptonidiso_syst"] = properSpacing("{sig_leptonidiso_syst}","1.06")
  d["sig_lumi_syst"] = properSpacing("{sig_lumi_syst}","1.026")
  d["sig_pileup_syst"] = properSpacing("{sig_pileup_syst}","1.03")
  d["sig_refacAndNorm_syst"] = properSpacing("{sig_refacAndNorm_syst}","1.03")

def getNuisenceParameters(SR):
  """Reads in the output of the plot maker for the signal region and collects all the key value pairs of nuisance parameters."""
#  f = open("outputs/configs_prediction_Final_%s.plots_out" % SR, "r")
#  f = open("/home/users/bsathian/ZMet/hists2017/DoubleLepton_narrow/DoubleLepton_narrow_signal/%s/DoubleLepton/statsplots.out"%SR,"r")
  f = open("/home/users/bsathian/ZMet/histsthreeyears/DoubleLepton/combined/statsplots_%s.out"%SR,"r")
  n_dict = {}
  for line in f:
    if re.match("{.*} [0-9]*\.[0-9]*\s$", line):
      toks=line.split()
      #If statements just switch the precision for different nuisences.
      if "rsfof*kappa" in toks[0]:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%.5f" % float(toks[1]))
      elif "count_" in toks[0] and "_fsbkg" in toks[0]:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%d" % int(float(toks[1])))
      else:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%.3f" % float(toks[1]))

  addConstantVals(n_dict)

  return n_dict

def makeDataCard(sp, SR, BR_key=None):
  if SR not in n_parms.keys():
    n_parms[SR] = getNuisenceParameters(SR)

  addSignalYields(n_parms[SR], SR, sp[0], sp[1], BR_key)

  #for x in n_parms[SR].keys():
  #  print("%s : %s" % (x, n_parms[SR][x]))

  if BR_key:
    outdir=output_path+BR_key+"/"
  else:
    outdir=output_path


  f_template=open("%s%s.txt" % (templates_path, SR), 'r')
  if (sp[1] == -1):
    f_out=open("%sdatacard_%s_mChi_%d_.txt" % (outdir, SR, sp[0]), 'w+')
  else:
    f_out=open("%sdatacard_%s_mGluino_%d_mLSP_%d_.txt" % (outdir, SR, sp[0], sp[1]), 'w+')

  f_out.write(f_template.read().format(**n_parms[SR]))

  f_out.close()
  f_template.close()

def launch():
  """Launches datacard making for each signal region depending on signal name"""
  print "signal region = ",signal_name
  for sp in mass_spectrum:
    if signal_name == "T5ZZ":
      makeDataCard(sp, "SRA")
      makeDataCard(sp, "SRAb")
      makeDataCard(sp, "SRB")
      makeDataCard(sp, "SRBb")
      makeDataCard(sp, "SRC")
      makeDataCard(sp, "SRCb")
    elif signal_name == "TChiWZ":
      makeDataCard(sp, "TChiHZ")
      makeDataCard(sp, "TChiWZ")
      makeDataCard(sp, "TChiHZ", "halfweight")
      makeDataCard(sp, "TChiWZ", "halfweight")
    elif signal_name == "tchiwz_ext":
      makeDataCard(sp, "TChiHZ")
      makeDataCard(sp, "TChiWZ")
    elif signal_name == "TChiZZ":
      makeDataCard((sp,-1), "TChiHZ")
      makeDataCard((sp,-1), "TChiWZ")
    elif signal_name == "TChiHZ":
      makeDataCard((sp,-1), "TChiHZ", "H100")
      makeDataCard((sp,-1), "TChiWZ", "H100")
      makeDataCard((sp,-1), "TChiHZ", "H90")
      makeDataCard((sp,-1), "TChiWZ", "H90")
      makeDataCard((sp,-1), "TChiHZ", "H80")
      makeDataCard((sp,-1), "TChiWZ", "H80")
      makeDataCard((sp,-1), "TChiHZ", "H70")
      makeDataCard((sp,-1), "TChiWZ", "H70")
      makeDataCard((sp,-1), "TChiHZ", "H60")
      makeDataCard((sp,-1), "TChiWZ", "H60")
      makeDataCard((sp,-1), "TChiHZ", "H50")
      makeDataCard((sp,-1), "TChiWZ", "H50")
      makeDataCard((sp,-1), "TChiHZ", "H40")
      makeDataCard((sp,-1), "TChiWZ", "H40")
      makeDataCard((sp,-1), "TChiHZ", "H30")
      makeDataCard((sp,-1), "TChiWZ", "H30")
      makeDataCard((sp,-1), "TChiHZ", "H20")
      makeDataCard((sp,-1), "TChiWZ", "H20")
      makeDataCard((sp,-1), "TChiHZ", "H10")
      makeDataCard((sp,-1), "TChiWZ", "H10")
      makeDataCard((sp,-1), "TChiHZ", "H0")
      makeDataCard((sp,-1), "TChiWZ", "H0")
    else:
      print("Do not know how to run on signal model %s. Please use t5zz or tchiwz." % signal_name)
      exit(1)

def setupVars():
  """Clears n_parms, sets histogram paths, mass spectrum, and output path for signal name"""
  global n_parms
  global mass_spectrum
  global histogram_Path
  global output_path
  n_parms = {}

  output_path="SMSScans/DataCards/%s/" % signal_name

  if signal_name == "T5ZZ":
    #histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/T5ZZScan/CV/"
    histogram_Path = "/home/users/bsathian/ZMet/histsthreeyears/fastsim/CV/combined/"
    mass_spectrum= [(800.0, 100.0), (800.0, 200.0), (800.0, 300.0), (800.0, 400.0), (800.0, 500.0), (800.0, 600.0), (800.0, 700.0), (800.0, 790.0), (900.0, 100.0), (900.0, 200.0), (900.0, 300.0), (900.0, 400.0), (900.0, 500.0), (900.0, 600.0), (900.0, 700.0), (900.0, 800.0), (900.0, 890.0), (1000.0, 100.0), (1000.0, 150.0), (1000.0, 200.0), (1000.0, 250.0), (1000.0, 300.0), (1000.0, 350.0), (1000.0, 400.0), (1000.0, 450.0), (1000.0, 500.0), (1000.0, 550.0), (1000.0, 600.0), (1000.0, 650.0), (1000.0, 700.0), (1000.0, 750.0), (1000.0, 800.0), (1000.0, 850.0), (1000.0, 900.0), (1000.0, 950.0), (1000.0, 990.0), (1050.0, 100.0), (1050.0, 150.0), (1050.0, 200.0), (1050.0, 250.0), (1050.0, 300.0), (1050.0, 350.0), (1050.0, 400.0), (1050.0, 450.0), (1050.0, 500.0), (1050.0, 550.0), (1050.0, 600.0), (1050.0, 650.0), (1050.0, 700.0), (1050.0, 750.0), (1050.0, 800.0), (1050.0, 850.0), (1050.0, 900.0), (1050.0, 950.0), (1050.0, 1000.0), (1050.0, 1040.0), (1100.0, 100.0), (1100.0, 150.0), (1100.0, 200.0), (1100.0, 250.0), (1100.0, 300.0), (1100.0, 350.0), (1100.0, 400.0), (1100.0, 450.0), (1100.0, 500.0), (1100.0, 550.0), (1100.0, 600.0), (1100.0, 650.0), (1100.0, 700.0), (1100.0, 750.0), (1100.0, 800.0), (1100.0, 850.0), (1100.0, 900.0), (1100.0, 950.0), (1100.0, 1000.0), (1100.0, 1050.0), (1100.0, 1090.0), (1150.0, 100.0), (1150.0, 150.0), (1150.0, 200.0), (1150.0, 250.0), (1150.0, 300.0), (1150.0, 350.0), (1150.0, 400.0), (1150.0, 450.0), (1150.0, 500.0), (1150.0, 550.0), (1150.0, 600.0), (1150.0, 650.0), (1150.0, 700.0), (1150.0, 750.0), (1150.0, 800.0), (1150.0, 850.0), (1150.0, 900.0), (1150.0, 950.0), (1150.0, 1000.0), (1150.0, 1050.0), (1150.0, 1100.0), (1150.0, 1140.0), (1200.0, 100.0), (1200.0, 150.0), (1200.0, 200.0), (1200.0, 250.0), (1200.0, 300.0), (1200.0, 350.0), (1200.0, 400.0), (1200.0, 450.0), (1200.0, 500.0), (1200.0, 550.0), (1200.0, 600.0), (1200.0, 650.0), (1200.0, 700.0), (1200.0, 750.0), (1200.0, 800.0), (1200.0, 850.0), (1200.0, 900.0), (1200.0, 950.0), (1200.0, 1000.0), (1200.0, 1050.0), (1200.0, 1100.0), (1200.0, 1150.0), (1200.0, 1190.0), (1250.0, 100.0), (1250.0, 150.0), (1250.0, 200.0), (1250.0, 250.0), (1250.0, 300.0), (1250.0, 350.0), (1250.0, 400.0), (1250.0, 450.0), (1250.0, 500.0), (1250.0, 550.0), (1250.0, 600.0), (1250.0, 650.0), (1250.0, 700.0), (1250.0, 750.0), (1250.0, 800.0), (1250.0, 850.0), (1250.0, 900.0), (1250.0, 950.0), (1250.0, 1000.0), (1250.0, 1050.0), (1250.0, 1100.0), (1250.0, 1150.0), (1250.0, 1200.0), (1250.0, 1240.0), (1300.0, 100.0), (1300.0, 150.0), (1300.0, 200.0), (1300.0, 250.0), (1300.0, 300.0), (1300.0, 350.0), (1300.0, 400.0), (1300.0, 450.0), (1300.0, 500.0), (1300.0, 550.0), (1300.0, 600.0), (1300.0, 650.0), (1300.0, 700.0), (1300.0, 750.0), (1300.0, 800.0), (1300.0, 850.0), (1300.0, 900.0), (1300.0, 950.0), (1300.0, 1000.0), (1300.0, 1050.0), (1300.0, 1100.0), (1300.0, 1150.0), (1300.0, 1200.0), (1300.0, 1250.0), (1300.0, 1290.0), (1350.0, 100.0), (1350.0, 150.0), (1350.0, 200.0), (1350.0, 250.0), (1350.0, 300.0), (1350.0, 350.0), (1350.0, 400.0), (1350.0, 450.0), (1350.0, 500.0), (1350.0, 550.0), (1350.0, 600.0), (1350.0, 650.0), (1350.0, 700.0), (1350.0, 750.0), (1350.0, 800.0), (1350.0, 850.0), (1350.0, 900.0), (1350.0, 950.0), (1350.0, 1000.0), (1350.0, 1050.0), (1350.0, 1100.0), (1350.0, 1150.0), (1350.0, 1200.0), (1350.0, 1250.0), (1350.0, 1300.0), (1350.0, 1340.0), (1400.0, 100.0), (1400.0, 150.0), (1400.0, 200.0), (1400.0, 250.0), (1400.0, 300.0), (1400.0, 350.0), (1400.0, 400.0), (1400.0, 450.0), (1400.0, 500.0), (1400.0, 550.0), (1400.0, 600.0), (1400.0, 650.0), (1400.0, 700.0), (1400.0, 750.0), (1400.0, 800.0), (1400.0, 850.0), (1400.0, 900.0), (1400.0, 950.0), (1400.0, 1000.0), (1400.0, 1050.0), (1400.0, 1100.0), (1400.0, 1150.0), (1400.0, 1200.0), (1400.0, 1250.0), (1400.0, 1300.0), (1400.0, 1350.0), (1400.0, 1390.0), (1450.0, 100.0), (1450.0, 150.0), (1450.0, 200.0), (1450.0, 250.0), (1450.0, 300.0), (1450.0, 350.0), (1450.0, 400.0), (1450.0, 450.0), (1450.0, 500.0), (1450.0, 550.0), (1450.0, 600.0), (1450.0, 650.0), (1450.0, 700.0), (1450.0, 750.0), (1450.0, 800.0), (1450.0, 850.0), (1450.0, 900.0), (1450.0, 950.0), (1450.0, 1000.0), (1450.0, 1050.0), (1450.0, 1100.0), (1450.0, 1150.0), (1450.0, 1200.0), (1450.0, 1250.0), (1450.0, 1300.0), (1450.0, 1350.0), (1450.0, 1400.0), (1450.0, 1440.0), (1500.0, 100.0), (1500.0, 150.0), (1500.0, 200.0), (1500.0, 250.0), (1500.0, 300.0), (1500.0, 350.0), (1500.0, 400.0), (1500.0, 450.0), (1500.0, 500.0), (1500.0, 550.0), (1500.0, 600.0), (1500.0, 650.0), (1500.0, 700.0), (1500.0, 750.0), (1500.0, 800.0), (1500.0, 850.0), (1500.0, 900.0), (1500.0, 950.0), (1500.0, 1000.0), (1500.0, 1050.0), (1500.0, 1100.0), (1500.0, 1150.0), (1500.0, 1200.0), (1500.0, 1250.0), (1500.0, 1300.0), (1500.0, 1350.0), (1500.0, 1400.0), (1500.0, 1450.0), (1500.0, 1490.0), (1550.0, 100.0), (1550.0, 150.0), (1550.0, 200.0), (1550.0, 250.0), (1550.0, 300.0), (1550.0, 350.0), (1550.0, 400.0), (1550.0, 450.0), (1550.0, 500.0), (1550.0, 550.0), (1550.0, 600.0), (1550.0, 650.0), (1550.0, 700.0), (1550.0, 750.0), (1550.0, 800.0), (1550.0, 850.0), (1550.0, 900.0), (1550.0, 950.0), (1550.0, 1000.0), (1550.0, 1050.0), (1550.0, 1100.0), (1550.0, 1150.0), (1550.0, 1200.0), (1550.0, 1250.0), (1550.0, 1300.0), (1550.0, 1350.0), (1550.0, 1400.0), (1550.0, 1450.0), (1550.0, 1500.0), (1550.0, 1540.0), (1600.0, 100.0), (1600.0, 150.0), (1600.0, 200.0), (1600.0, 250.0), (1600.0, 300.0), (1600.0, 350.0), (1600.0, 400.0), (1600.0, 450.0), (1600.0, 500.0), (1600.0, 550.0), (1600.0, 600.0), (1600.0, 650.0), (1600.0, 700.0), (1600.0, 750.0), (1600.0, 800.0), (1600.0, 850.0), (1600.0, 900.0), (1600.0, 950.0), (1600.0, 1000.0), (1600.0, 1050.0), (1600.0, 1100.0), (1600.0, 1150.0), (1600.0, 1200.0), (1600.0, 1250.0), (1600.0, 1300.0), (1600.0, 1350.0), (1600.0, 1400.0), (1600.0, 1450.0), (1600.0, 1500.0), (1600.0, 1550.0), (1600.0, 1590.0), (1650.0, 100.0), (1650.0, 150.0), (1650.0, 200.0), (1650.0, 250.0), (1650.0, 300.0), (1650.0, 350.0), (1650.0, 400.0), (1650.0, 450.0), (1650.0, 500.0), (1650.0, 550.0), (1650.0, 600.0), (1650.0, 650.0), (1650.0, 700.0), (1650.0, 750.0), (1650.0, 800.0), (1650.0, 850.0), (1650.0, 900.0), (1650.0, 950.0), (1650.0, 1000.0), (1650.0, 1050.0), (1650.0, 1100.0), (1650.0, 1150.0), (1650.0, 1200.0), (1650.0, 1250.0), (1650.0, 1300.0), (1650.0, 1350.0), (1650.0, 1400.0), (1650.0, 1450.0), (1650.0, 1500.0), (1650.0, 1550.0), (1650.0, 1600.0), (1650.0, 1640.0), (1700.0, 100.0), (1700.0, 150.0), (1700.0, 200.0), (1700.0, 250.0), (1700.0, 300.0), (1700.0, 350.0), (1700.0, 400.0), (1700.0, 450.0), (1700.0, 500.0), (1700.0, 550.0), (1700.0, 600.0), (1700.0, 650.0), (1700.0, 700.0), (1700.0, 750.0), (1700.0, 800.0), (1700.0, 850.0), (1700.0, 900.0), (1700.0, 950.0), (1700.0, 1000.0), (1700.0, 1050.0), (1700.0, 1100.0), (1700.0, 1150.0), (1700.0, 1200.0), (1700.0, 1250.0), (1700.0, 1300.0), (1700.0, 1350.0), (1700.0, 1400.0), (1700.0, 1450.0), (1700.0, 1500.0), (1700.0, 1550.0), (1700.0, 1600.0), (1700.0, 1650.0), (1700.0, 1690.0), (1750.0, 100.0), (1750.0, 150.0), (1750.0, 200.0), (1750.0, 250.0), (1750.0, 300.0), (1750.0, 350.0), (1750.0, 400.0), (1750.0, 450.0), (1750.0, 500.0), (1750.0, 550.0), (1750.0, 600.0), (1750.0, 650.0), (1750.0, 700.0), (1750.0, 750.0), (1750.0, 800.0), (1750.0, 850.0), (1750.0, 900.0), (1750.0, 950.0), (1750.0, 1000.0), (1750.0, 1050.0), (1750.0, 1100.0), (1750.0, 1150.0), (1750.0, 1200.0), (1750.0, 1250.0), (1750.0, 1300.0), (1750.0, 1350.0), (1750.0, 1400.0), (1750.0, 1450.0), (1750.0, 1500.0), (1750.0, 1550.0), (1750.0, 1600.0), (1750.0, 1650.0), (1750.0, 1700.0), (1750.0, 1740.0), (1800.0, 100.0), (1800.0, 150.0), (1800.0, 200.0), (1800.0, 250.0), (1800.0, 300.0), (1800.0, 350.0), (1800.0, 400.0), (1800.0, 450.0), (1800.0, 500.0), (1800.0, 550.0), (1800.0, 600.0), (1800.0, 650.0), (1800.0, 700.0), (1800.0, 750.0), (1800.0, 800.0), (1800.0, 850.0), (1800.0, 900.0), (1800.0, 950.0), (1800.0, 1000.0), (1800.0, 1050.0), (1800.0, 1100.0), (1800.0, 1150.0), (1800.0, 1200.0), (1800.0, 1250.0), (1800.0, 1300.0), (1800.0, 1350.0), (1800.0, 1400.0), (1800.0, 1450.0), (1800.0, 1500.0), (1800.0, 1550.0), (1800.0, 1600.0), (1800.0, 1650.0), (1800.0, 1700.0), (1800.0, 1750.0), (1800.0, 1790.0), (1850.0, 100.0), (1850.0, 150.0), (1850.0, 200.0), (1850.0, 250.0), (1850.0, 300.0), (1850.0, 350.0), (1850.0, 400.0), (1850.0, 450.0), (1850.0, 500.0), (1850.0, 550.0), (1850.0, 600.0), (1850.0, 650.0), (1850.0, 700.0), (1850.0, 750.0), (1850.0, 800.0), (1850.0, 850.0), (1850.0, 900.0), (1850.0, 950.0), (1850.0, 1000.0), (1850.0, 1050.0), (1850.0, 1100.0), (1850.0, 1150.0), (1850.0, 1200.0), (1850.0, 1250.0), (1850.0, 1300.0), (1850.0, 1350.0), (1850.0, 1400.0), (1850.0, 1450.0), (1850.0, 1500.0), (1850.0, 1550.0), (1850.0, 1600.0), (1850.0, 1650.0), (1850.0, 1700.0), (1850.0, 1750.0), (1850.0, 1800.0), (1850.0, 1840.0), (1900.0, 100.0), (1900.0, 150.0), (1900.0, 200.0), (1900.0, 250.0), (1900.0, 300.0), (1900.0, 350.0), (1900.0, 400.0), (1900.0, 450.0), (1900.0, 500.0), (1900.0, 550.0), (1900.0, 600.0), (1900.0, 650.0), (1900.0, 700.0), (1900.0, 750.0), (1900.0, 800.0), (1900.0, 850.0), (1900.0, 900.0), (1900.0, 950.0), (1900.0, 1000.0), (1900.0, 1050.0), (1900.0, 1100.0), (1900.0, 1150.0), (1900.0, 1200.0), (1900.0, 1250.0), (1900.0, 1300.0), (1900.0, 1350.0), (1900.0, 1400.0), (1900.0, 1450.0), (1900.0, 1500.0), (1900.0, 1550.0), (1900.0, 1600.0), (1900.0, 1650.0), (1900.0, 1700.0), (1900.0, 1750.0), (1900.0, 1800.0), (1900.0, 1850.0), (1900.0, 1890.0), (1950.0, 100.0), (1950.0, 150.0), (1950.0, 200.0), (1950.0, 250.0), (1950.0, 300.0), (1950.0, 350.0), (1950.0, 400.0), (1950.0, 450.0), (1950.0, 500.0), (1950.0, 550.0), (1950.0, 600.0), (1950.0, 650.0), (1950.0, 700.0), (1950.0, 750.0), (1950.0, 800.0), (1950.0, 850.0), (1950.0, 900.0), (1950.0, 950.0), (1950.0, 1000.0), (1950.0, 1050.0), (1950.0, 1100.0), (1950.0, 1150.0), (1950.0, 1200.0), (1950.0, 1250.0), (1950.0, 1300.0), (1950.0, 1350.0), (1950.0, 1400.0), (1950.0, 1450.0), (1950.0, 1500.0), (1950.0, 1550.0), (1950.0, 1600.0), (1950.0, 1650.0), (1950.0, 1700.0), (1950.0, 1750.0), (1950.0, 1800.0), (1950.0, 1850.0), (1950.0, 1900.0), (1950.0, 1940.0), (2000.0, 100.0), (2000.0, 150.0), (2000.0, 200.0), (2000.0, 250.0), (2000.0, 300.0), (2000.0, 350.0), (2000.0, 400.0), (2000.0, 450.0), (2000.0, 500.0), (2000.0, 550.0), (2000.0, 600.0), (2000.0, 650.0), (2000.0, 700.0), (2000.0, 750.0), (2000.0, 800.0), (2000.0, 850.0), (2000.0, 900.0), (2000.0, 950.0), (2000.0, 1000.0), (2000.0, 1050.0), (2000.0, 1100.0), (2000.0, 1150.0), (2000.0, 1200.0), (2000.0, 1250.0), (2000.0, 1300.0), (2000.0, 1350.0), (2000.0, 1400.0), (2000.0, 1450.0), (2000.0, 1500.0), (2000.0, 1550.0), (2000.0, 1600.0), (2000.0, 1650.0), (2000.0, 1700.0), (2000.0, 1750.0), (2000.0, 1800.0), (2000.0, 1850.0), (2000.0, 1900.0), (2000.0, 1950.0), (2000.0, 1990.0), (2050.0, 100.0), (2050.0, 150.0), (2050.0, 200.0), (2050.0, 250.0), (2050.0, 300.0), (2050.0, 350.0), (2050.0, 400.0), (2050.0, 450.0), (2050.0, 500.0), (2050.0, 550.0), (2050.0, 600.0), (2050.0, 650.0), (2050.0, 700.0), (2050.0, 750.0), (2050.0, 800.0), (2050.0, 850.0), (2050.0, 900.0), (2050.0, 950.0), (2050.0, 1000.0), (2050.0, 1050.0), (2050.0, 1100.0), (2050.0, 1150.0), (2050.0, 1200.0), (2050.0, 1250.0), (2050.0, 1300.0), (2050.0, 1350.0), (2050.0, 1400.0), (2050.0, 1450.0), (2050.0, 1500.0), (2050.0, 1550.0), (2050.0, 1600.0), (2050.0, 1650.0), (2050.0, 1700.0), (2050.0, 1750.0), (2050.0, 1800.0), (2050.0, 1850.0), (2050.0, 1900.0), (2050.0, 1950.0), (2050.0, 2000.0), (2050.0, 2040.0), (2100.0, 100.0), (2100.0, 150.0), (2100.0, 200.0), (2100.0, 250.0), (2100.0, 300.0), (2100.0, 350.0), (2100.0, 400.0), (2100.0, 450.0), (2100.0, 500.0), (2100.0, 550.0), (2100.0, 600.0), (2100.0, 650.0), (2100.0, 700.0), (2100.0, 750.0), (2100.0, 800.0), (2100.0, 850.0), (2100.0, 900.0), (2100.0, 950.0), (2100.0, 1000.0), (2100.0, 1050.0), (2100.0, 1100.0), (2100.0, 1150.0), (2100.0, 1200.0), (2100.0, 1250.0), (2100.0, 1300.0), (2100.0, 1350.0), (2100.0, 1400.0), (2100.0, 1450.0), (2100.0, 1500.0), (2100.0, 1550.0), (2100.0, 1600.0), (2100.0, 1650.0), (2100.0, 1700.0), (2100.0, 1750.0), (2100.0, 1800.0), (2100.0, 1850.0), (2100.0, 1900.0), (2100.0, 1950.0), (2100.0, 2000.0), (2100.0, 2050.0), (2150.0, 100.0), (2150.0, 150.0), (2150.0, 200.0), (2150.0, 250.0), (2150.0, 300.0), (2150.0, 350.0), (2150.0, 400.0), (2150.0, 450.0), (2150.0, 500.0), (2150.0, 550.0), (2150.0, 600.0), (2150.0, 650.0), (2150.0, 700.0), (2150.0, 750.0), (2150.0, 800.0), (2150.0, 850.0), (2150.0, 900.0), (2150.0, 950.0), (2150.0, 1000.0), (2150.0, 1050.0), (2150.0, 1100.0), (2150.0, 1150.0), (2150.0, 1200.0), (2150.0, 1250.0), (2150.0, 1300.0), (2150.0, 1350.0), (2150.0, 1400.0), (2150.0, 1450.0), (2150.0, 1500.0), (2150.0, 1550.0), (2150.0, 1600.0), (2150.0, 1650.0), (2150.0, 1700.0), (2150.0, 1750.0), (2150.0, 1800.0), (2150.0, 1850.0), (2150.0, 1900.0), (2150.0, 1950.0), (2150.0, 2000.0), (2150.0, 2050.0), (2150.0, 2100.0), (2200.0, 100.0), (2200.0, 150.0), (2200.0, 200.0), (2200.0, 250.0), (2200.0, 300.0), (2200.0, 350.0), (2200.0, 400.0), (2200.0, 450.0), (2200.0, 500.0), (2200.0, 550.0), (2200.0, 600.0), (2200.0, 650.0), (2200.0, 700.0), (2200.0, 750.0), (2200.0, 800.0), (2200.0, 850.0), (2200.0, 900.0), (2200.0, 950.0), (2200.0, 1000.0), (2200.0, 1050.0), (2200.0, 1100.0), (2200.0, 1150.0), (2200.0, 1200.0), (2200.0, 1250.0), (2200.0, 1300.0), (2200.0, 1350.0), (2200.0, 1400.0), (2200.0, 1450.0), (2200.0, 1500.0), (2200.0, 1550.0), (2200.0, 1600.0), (2200.0, 1650.0), (2200.0, 1700.0), (2200.0, 1750.0), (2200.0, 1800.0), (2200.0, 1850.0), (2200.0, 1900.0), (2200.0, 1950.0), (2200.0, 2000.0), (2200.0, 2050.0), (2200.0, 2100.0), (2200.0, 2150.0), (2250.0, 100.0), (2250.0, 150.0), (2250.0, 200.0), (2250.0, 250.0), (2250.0, 300.0), (2250.0, 350.0), (2250.0, 400.0), (2250.0, 450.0), (2250.0, 500.0), (2250.0, 550.0), (2250.0, 600.0), (2250.0, 650.0), (2250.0, 700.0), (2250.0, 750.0), (2250.0, 800.0), (2250.0, 850.0), (2250.0, 900.0), (2250.0, 950.0), (2250.0, 1000.0), (2250.0, 1050.0), (2250.0, 1100.0), (2250.0, 1150.0), (2250.0, 1200.0), (2250.0, 1250.0), (2250.0, 1300.0), (2250.0, 1350.0), (2250.0, 1400.0), (2250.0, 1450.0), (2250.0, 1500.0), (2250.0, 1550.0), (2250.0, 1600.0), (2250.0, 1650.0), (2250.0, 1700.0), (2250.0, 1750.0), (2250.0, 1800.0), (2250.0, 1850.0), (2250.0, 1900.0), (2250.0, 1950.0), (2250.0, 2000.0), (2250.0, 2050.0), (2250.0, 2100.0), (2250.0, 2150.0), (2250.0, 2200.0), (2300.0, 100.0), (2300.0, 150.0), (2300.0, 200.0), (2300.0, 250.0), (2300.0, 300.0), (2300.0, 350.0), (2300.0, 400.0), (2300.0, 450.0), (2300.0, 500.0), (2300.0, 550.0), (2300.0, 600.0), (2300.0, 650.0), (2300.0, 700.0), (2300.0, 750.0), (2300.0, 800.0), (2300.0, 850.0), (2300.0, 900.0), (2300.0, 950.0), (2300.0, 1000.0), (2300.0, 1050.0), (2300.0, 1100.0), (2300.0, 1150.0), (2300.0, 1200.0), (2300.0, 1250.0), (2300.0, 1300.0), (2300.0, 1350.0), (2300.0, 1400.0), (2300.0, 1450.0), (2300.0, 1500.0), (2300.0, 1550.0), (2300.0, 1600.0), (2300.0, 1650.0), (2300.0, 1700.0), (2300.0, 1750.0), (2300.0, 1800.0), (2300.0, 1850.0), (2300.0, 1900.0), (2300.0, 1950.0), (2300.0, 2000.0), (2300.0, 2050.0), (2300.0, 2100.0), (2300.0, 2150.0), (2300.0, 2200.0), (2300.0, 2250.0), (2350.0, 100.0), (2350.0, 150.0), (2350.0, 200.0), (2350.0, 250.0), (2350.0, 300.0), (2350.0, 350.0), (2350.0, 400.0), (2350.0, 450.0), (2350.0, 500.0), (2350.0, 550.0), (2350.0, 600.0), (2350.0, 650.0), (2350.0, 700.0), (2350.0, 750.0), (2350.0, 800.0), (2350.0, 850.0), (2350.0, 900.0), (2350.0, 950.0), (2350.0, 1000.0), (2350.0, 1050.0), (2350.0, 1100.0), (2350.0, 1150.0), (2350.0, 1200.0), (2350.0, 1250.0), (2350.0, 1300.0), (2350.0, 1350.0), (2350.0, 1400.0), (2350.0, 1450.0), (2350.0, 1500.0), (2350.0, 1550.0), (2350.0, 1600.0), (2350.0, 1650.0), (2350.0, 1700.0), (2350.0, 1750.0), (2350.0, 1800.0), (2350.0, 1850.0), (2350.0, 1900.0), (2350.0, 1950.0), (2350.0, 2000.0), (2350.0, 2050.0), (2350.0, 2100.0), (2350.0, 2150.0), (2350.0, 2200.0), (2350.0, 2250.0), (2350.0, 2300.0)]
  elif signal_name == "tchiwz":
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/TChiWZScan/CV/"
    mass_spectrum=[(350.000000,1.000000),(600.000000,200.000000),(550.000000,1.000000),(225.000000,125.000000),(700.000000,25.000000),(575.000000,225.000000),(675.000000,25.000000),(575.000000,275.000000),(250.000000,75.000000),(350.000000,240.000000),(125.000000,35.000000),(150.000000,10.000000),(425.000000,225.000000),(700.000000,75.000000),(400.000000,260.000000),(275.000000,145.000000),(500.000000,225.000000),(150.000000,70.000000),(500.000000,300.000000),(300.000000,270.000000),(125.000000,75.000000),(600.000000,1.000000),(325.000000,225.000000),(675.000000,100.000000),(600.000000,275.000000),(700.000000,100.000000),(450.000000,200.000000),(500.000000,75.000000),(475.000000,75.000000),(475.000000,150.000000),(375.000000,25.000000),(200.000000,185.000000),(500.000000,150.000000),(425.000000,150.000000),(400.000000,25.000000),(550.000000,100.000000),(175.000000,85.000000),(300.000000,290.000000),(575.000000,100.000000),(375.000000,75.000000),(275.000000,185.000000),(675.000000,250.000000),(650.000000,200.000000),(400.000000,100.000000),(200.000000,160.000000),(150.000000,80.000000),(700.000000,200.000000),(325.000000,175.000000),(475.000000,50.000000),(225.000000,135.000000),(575.000000,250.000000),(250.000000,100.000000),(425.000000,50.000000),(575.000000,75.000000),(325.000000,100.000000),(200.000000,193.000000),(500.000000,275.000000),(100.000000,20.000000),(700.000000,275.000000),(225.000000,155.000000),(650.000000,275.000000),(450.000000,1.000000),(550.000000,25.000000),(650.000000,1.000000),(150.000000,60.000000),(275.000000,225.000000),(625.000000,50.000000),(225.000000,75.000000),(650.000000,250.000000),(550.000000,175.000000),(200.000000,180.000000),(550.000000,300.000000),(250.000000,140.000000),(450.000000,50.000000),(475.000000,175.000000),(375.000000,245.000000),(200.000000,70.000000),(100.000000,60.000000),(200.000000,50.000000),(325.000000,1.000000),(600.000000,250.000000),(500.000000,25.000000),(475.000000,25.000000),(275.000000,100.000000),(300.000000,285.000000),(250.000000,210.000000),(550.000000,275.000000),(675.000000,150.000000),(700.000000,125.000000),(275.000000,260.000000),(225.000000,210.000000),(125.000000,45.000000),(300.000000,125.000000),(100.000000,1.000000),(475.000000,225.000000),(250.000000,180.000000),(225.000000,205.000000),(275.000000,135.000000),(200.000000,110.000000),(200.000000,170.000000),(175.000000,168.000000),(525.000000,200.000000),(375.000000,225.000000),(200.000000,90.000000),(150.000000,140.000000),(375.000000,150.000000),(450.000000,75.000000),(400.000000,250.000000),(325.000000,150.000000),(525.000000,150.000000),(350.000000,200.000000),(100.000000,90.000000),(150.000000,50.000000),(350.000000,210.000000),(625.000000,175.000000),(150.000000,20.000000),(300.000000,250.000000),(250.000000,220.000000),(350.000000,125.000000),(275.000000,175.000000),(150.000000,135.000000),(125.000000,25.000000),(125.000000,115.000000),(250.000000,160.000000),(150.000000,90.000000),(325.000000,185.000000),(350.000000,260.000000),(325.000000,195.000000),(175.000000,55.000000),(525.000000,250.000000),(350.000000,100.000000),(150.000000,1.000000),(150.000000,30.000000),(525.000000,75.000000),(375.000000,100.000000),(400.000000,75.000000),(475.000000,275.000000),(500.000000,125.000000),(350.000000,25.000000),(425.000000,275.000000),(625.000000,200.000000),(300.000000,180.000000),(425.000000,1.000000),(350.000000,220.000000),(525.000000,25.000000),(375.000000,285.000000),(500.000000,1.000000),(300.000000,210.000000),(700.000000,1.000000),(300.000000,75.000000),(100.000000,50.000000),(275.000000,215.000000),(350.000000,230.000000),(275.000000,50.000000),(600.000000,50.000000),(525.000000,1.000000),(375.000000,125.000000),(250.000000,200.000000),(125.000000,5.000000),(575.000000,50.000000),(200.000000,1.000000),(400.000000,290.000000),(600.000000,100.000000),(575.000000,300.000000),(375.000000,175.000000),(350.000000,280.000000),(100.000000,93.000000),(175.000000,35.000000),(350.000000,290.000000),(525.000000,125.000000),(500.000000,250.000000),(225.000000,195.000000),(425.000000,285.000000),(375.000000,1.000000),(625.000000,250.000000),(300.000000,170.000000),(575.000000,1.000000),(275.000000,255.000000),(325.000000,275.000000),(525.000000,275.000000),(250.000000,240.000000),(375.000000,295.000000),(350.000000,250.000000),(675.000000,75.000000),(600.000000,175.000000),(175.000000,125.000000),(175.000000,135.000000),(250.000000,25.000000),(550.000000,50.000000),(350.000000,175.000000),(225.000000,25.000000),(275.000000,125.000000),(300.000000,260.000000),(400.000000,175.000000),(600.000000,225.000000),(200.000000,100.000000),(350.000000,300.000000),(325.000000,235.000000),(625.000000,1.000000),(250.000000,235.000000),(300.000000,100.000000),(325.000000,245.000000),(650.000000,225.000000),(500.000000,200.000000),(200.000000,80.000000),(625.000000,125.000000),(350.000000,150.000000),(425.000000,125.000000),(450.000000,150.000000),(550.000000,150.000000),(550.000000,225.000000),(650.000000,25.000000),(100.000000,80.000000),(350.000000,75.000000),(225.000000,95.000000),(375.000000,275.000000),(575.000000,175.000000),(275.000000,165.000000),(400.000000,225.000000),(175.000000,45.000000),(600.000000,300.000000),(400.000000,270.000000),(200.000000,140.000000),(650.000000,175.000000),(675.000000,50.000000),(450.000000,300.000000),(500.000000,175.000000),(350.000000,50.000000),(675.000000,275.000000),(200.000000,120.000000),(625.000000,275.000000),(250.000000,50.000000),(475.000000,250.000000),(450.000000,275.000000),(175.000000,95.000000),(275.000000,235.000000),(125.000000,118.000000),(325.000000,25.000000),(475.000000,1.000000),(100.000000,70.000000),(675.000000,1.000000),(300.000000,293.000000),(150.000000,40.000000),(275.000000,205.000000),(175.000000,155.000000),(225.000000,218.000000),(150.000000,100.000000),(125.000000,15.000000),(700.000000,50.000000),(250.000000,1.000000),(125.000000,105.000000),(250.000000,150.000000),(650.000000,50.000000),(300.000000,50.000000),(275.000000,75.000000),(175.000000,25.000000),(425.000000,200.000000),(175.000000,160.000000),(625.000000,25.000000),(475.000000,200.000000),(450.000000,250.000000),(250.000000,120.000000),(225.000000,175.000000),(175.000000,75.000000),(275.000000,245.000000),(375.000000,200.000000),(700.000000,150.000000),(650.000000,150.000000),(650.000000,75.000000),(100.000000,10.000000),(150.000000,143.000000),(650.000000,300.000000),(675.000000,200.000000),(250.000000,190.000000),(425.000000,100.000000),(700.000000,300.000000),(450.000000,100.000000),(125.000000,85.000000),(250.000000,130.000000),(500.000000,50.000000),(575.000000,200.000000),(375.000000,255.000000),(250.000000,110.000000),(100.000000,40.000000),(425.000000,75.000000),(700.000000,225.000000),(175.000000,115.000000),(175.000000,165.000000),(300.000000,1.000000),(225.000000,185.000000),(225.000000,165.000000),(400.000000,150.000000),(450.000000,25.000000),(125.000000,110.000000),(425.000000,25.000000),(525.000000,225.000000),(325.000000,75.000000),(300.000000,240.000000),(650.000000,125.000000),(250.000000,230.000000),(525.000000,175.000000),(275.000000,155.000000),(450.000000,225.000000),(250.000000,170.000000),(425.000000,295.000000),(225.000000,105.000000),(200.000000,25.000000),(200.000000,130.000000),(350.000000,270.000000),(475.000000,300.000000),(325.000000,205.000000),(125.000000,65.000000),(100.000000,85.000000),(325.000000,215.000000),(400.000000,50.000000),(650.000000,100.000000),(550.000000,125.000000),(375.000000,235.000000),(400.000000,200.000000),(175.000000,1.000000),(275.000000,25.000000),(625.000000,150.000000),(425.000000,250.000000),(325.000000,265.000000),(600.000000,125.000000),(625.000000,75.000000),(400.000000,125.000000),(575.000000,150.000000),(575.000000,25.000000),(375.000000,265.000000),(300.000000,200.000000),(525.000000,100.000000),(300.000000,230.000000),(550.000000,75.000000),(275.000000,268.000000),(275.000000,195.000000),(625.000000,300.000000),(675.000000,125.000000),(175.000000,145.000000),(625.000000,225.000000),(150.000000,110.000000),(525.000000,50.000000),(400.000000,280.000000),(225.000000,1.000000),(425.000000,175.000000),(200.000000,150.000000),(600.000000,25.000000),(700.000000,175.000000),(250.000000,243.000000),(150.000000,130.000000),(575.000000,125.000000),(550.000000,200.000000),(325.000000,255.000000),(600.000000,75.000000),(100.000000,30.000000),(450.000000,125.000000),(300.000000,160.000000),(300.000000,25.000000),(225.000000,145.000000),(175.000000,65.000000),(400.000000,1.000000),(225.000000,115.000000),(300.000000,190.000000),(325.000000,285.000000),(325.000000,295.000000),(450.000000,175.000000),(125.000000,55.000000),(700.000000,250.000000),(325.000000,125.000000),(625.000000,100.000000),(300.000000,220.000000),(675.000000,300.000000),(150.000000,120.000000),(200.000000,190.000000),(475.000000,100.000000),(200.000000,60.000000),(500.000000,100.000000),(375.000000,50.000000),(400.000000,300.000000),(675.000000,175.000000),(525.000000,300.000000),(225.000000,85.000000),(125.000000,95.000000),(325.000000,50.000000),(300.000000,280.000000),(600.000000,150.000000),(550.000000,250.000000),(275.000000,265.000000),(225.000000,215.000000),(675.000000,225.000000),(225.000000,50.000000),(175.000000,105.000000),(275.000000,1.000000),(300.000000,150.000000),(475.000000,125.000000)]
  elif signal_name == "tchiwz_ext":
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/TChiWZ_EXTScan/CV/"
    mass_spectrum=[(700.000000,325.000000),(600.000000,350.000000),(475.000000,345.000000),(650.000000,350.000000),(475.000000,325.000000),(425.000000,335.000000),(475.000000,335.000000),(600.000000,300.000000),(625.000000,350.000000),(400.000000,310.000000),(400.000000,320.000000),(325.000000,315.000000),(525.000000,350.000000),(550.000000,300.000000),(550.000000,350.000000),(700.000000,350.000000),(350.000000,300.000000),(650.000000,325.000000),(675.000000,300.000000),(350.000000,320.000000),(400.000000,300.000000),(450.000000,350.000000),(350.000000,343.000000),(525.000000,325.000000),(675.000000,350.000000),(350.000000,310.000000),(450.000000,310.000000),(375.000000,325.000000),(425.000000,305.000000),(400.000000,350.000000),(375.000000,315.000000),(650.000000,300.000000),(550.000000,325.000000),(450.000000,300.000000),(325.000000,318.000000),(525.000000,300.000000),(575.000000,325.000000),(350.000000,340.000000),(400.000000,340.000000),(500.000000,350.000000),(450.000000,330.000000),(450.000000,340.000000),(350.000000,330.000000),(375.000000,335.000000),(325.000000,310.000000),(425.000000,325.000000),(700.000000,300.000000),(375.000000,345.000000),(625.000000,325.000000),(500.000000,300.000000),(350.000000,335.000000),(400.000000,330.000000),(375.000000,305.000000),(325.000000,305.000000),(475.000000,300.000000),(425.000000,315.000000),(575.000000,300.000000),(450.000000,320.000000),(600.000000,325.000000),(625.000000,300.000000),(500.000000,325.000000),(575.000000,350.000000),(675.000000,325.000000),(425.000000,345.000000)]
  elif (signal_name == "tchizz"):
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/TChiZZScan/CV/"
    mass_spectrum = [100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000]
  elif (signal_name == "tchihz"):
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/TChiHZScan/CV/"
    mass_spectrum = [127,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000]
  else:
    print("Do not know how to run on signal model %s. Please use t5zz or tchiwz." % signal_name)
    exit(1)


def main():
  global signal_name, multiplier
  parser = argparse.ArgumentParser()

  parser.add_argument("--t5zz",       help="make datacards for t5zz sample", action="store_true")
  parser.add_argument("--tchiwz",     help="make datacards for TChiWZ sample", action="store_true")
  parser.add_argument("--tchiwz_ext", help="make datacards for TChiWZ sample  (constantin's extention)", action="store_true")
  parser.add_argument("--tchizz",     help="make datacards for TChiZZ sample", action="store_true")
  parser.add_argument("--tchihz",     help="make datacards for TChiHZ sample", action="store_true")

  args=parser.parse_args()

  if (args.t5zz):
    signal_name = "T5ZZ"
  elif (args.tchiwz):
    signal_name = "tchiwz"
  elif (args.tchiwz_ext):
    signal_name = "tchiwz_ext"
  elif (args.tchizz):
    signal_name = "tchizz"
  elif (args.tchihz):
    signal_name = "tchihz"
  else:
    parser.print_help()
    exit()

  setupVars()
  launch()

if __name__=="__main__":
  main()
