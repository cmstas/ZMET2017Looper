#!/usr/bin/env python

import argparse, sys, re


signal_chain="/nfs-7/"
templates_path="SMSScans/Templates/"

n_parms = {}

def getNuisenceParameters(SR):
  f = open("outputs/configs_prediction_Final_%s.plots_out" % SR, "r")
  
  n_dict = {}
  for line in f:
    if re.match("{.*} [0-9]\.[0-9]*\s$", line):
      toks=line.split()
      n_dict[toks[0]] = toks[1]

  return n_dict

def makeDataCard(sp, SR):
  if SR not in n_parms.keys():
    n_parms[SR] = getNuisenceParameters(SR)

  for x in n_parms[SR].keys():
    print("%s : %s" & (x, n_parms[SR][x]))

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