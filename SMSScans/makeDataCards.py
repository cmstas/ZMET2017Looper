#!/usr/bin/env python

import argparse, sys, re, getSignalNumbers


signal_chain="/nfs-7/"
#histogram_Path="/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/T5ZZScan/"
histogram_Path="~/"
signal_name="t5zz"

templates_path="SMSScans/Templates/"
output_path="SMSScans/DataCards/"

n_parms = {}

def properSpacing(key, param):
  """return param padded with spaces so that it's length {key} to preserve tabbing in templates"""

  delta=len(key)-len(param)

  if (len(param) < len(key)):
    param=(" "*delta)+param

  return param

def addSignalYields(d, SR):

  yields=getSignalNumbers.getSignalYields(SR, "%s%s/%s.root" % (histogram_Path, SR, signal_name))

  for i,y in enumerate(yields):
    d["BGbin%d_sig" % i] = properSpacing("{BGbin1_sig}", "%.2f" % y)


def addConstantVals(d):
  d["sig_trig_syst"] = properSpacing("{sig_trig_syst}","0.04")
  d["sig_metfromFS_syst_bin1"] = properSpacing("{sig_metfromFS_syst_bin1}","0.04")
  d["sig_metfromFS_syst_bin2"] = properSpacing("{sig_metfromFS_syst_bin2}","0.04")
  d["sig_metfromFS_syst_bin3"] = properSpacing("{sig_metfromFS_syst_bin3}","0.04")
  d["sig_leptonFS_syst"] = properSpacing("{sig_leptonFS_syst}","0.04")
  d["sig_btagheavy_syst"] = properSpacing("{sig_btagheavy_syst}","0.04")
  d["sig_btaglight_syst"] = properSpacing("{sig_btaglight_syst}","0.04")
  d["sig_lumi_syst"] = properSpacing("{sig_lumi_syst}","0.04")
  d["sig_isr_syst"] = properSpacing("{sig_isr_syst}","0.04")
  d["sig_JES_syst_bin1"] = properSpacing("{sig_JES_syst_bin1}","0.04")
  d["sig_JES_syst_bin2"] = properSpacing("{sig_JES_syst_bin2}","0.04")
  d["sig_JES_syst_bin3"] = properSpacing("{sig_JES_syst_bin3}","0.04")

  d["sig_stat_syst_bin1"] = properSpacing("{sig_stat_syst_bin1}","0.04")
  d["sig_stat_syst_bin2"] = properSpacing("{sig_stat_syst_bin2}","0.04")
  d["sig_stat_syst_bin3"] = properSpacing("{sig_stat_syst_bin3}","0.04")


def getNuisenceParameters(SR):
  """Reads in the output of the plot maker for the signal region and collects all the key value pairs of nuisance parameters."""
  f = open("outputs/configs_prediction_Final_%s.plots_out" % SR, "r")
  
  n_dict = {}
  for line in f:
    if re.match("{.*} [0-9]*\.[0-9]*\s$", line):
      toks=line.split()
      n_dict[toks[0][1:-1]] = properSpacing(toks[0],toks[1])

  addConstantVals(n_dict)
  addSignalYields(n_dict, SR)

  return n_dict

def makeDataCard(sp, SR):
  if SR not in n_parms.keys():
    n_parms[SR] = getNuisenceParameters(SR)

  for x in n_parms[SR].keys():
    print("%s : %s" % (x, n_parms[SR][x]))

  f_template=open("%s%s.txt" % (templates_path, SR), 'r')
  f_out=open("%s%s_mglu%s_mlsp_%s.txt" % (output_path, SR, sp[0], sp[1]), 'w+')

  f_out.write(f_template.read().format(**n_parms[SR]))

  f_out.close()
  f_template.close()


def main():
  #mass_spectrum = getMassSpectrumFromTChain(signal_chain)
  makeDataCard((100,200), "SRAb")

  #for sp in mass_spectrum:
    #makeDataCard(sp, "SRA")
    #makeDataCard(sp, "SRAb")
    #makeDataCard(sp, "SRB")
    #makeDataCard(sp, "SRBb")
    #makeDataCard(sp, "SRC")
    #makeDataCard(sp, "SRCb")
    #makeDataCard(sp, "TChiHZ")
    #makeDataCard(sp, "TChiWZ")


if __name__=="__main__":
  main()