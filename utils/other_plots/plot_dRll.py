import numpy as np
import ROOT as r
import plottery.plottery as ply
import sys,os

def combine_histograms(ee_hist,mm_hist,SR):
    final_hist = ee_hist.Clone(SR+"final_hist")
    final_hist.Add(mm_hist)
    return final_hist


hist_prefix = "/home/users/bsathian/ZMet/histsthreeyears/Final/DoubleLepton/combined/"
SRs = ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ"]
norm_scale_factors = {"SRA":0.052152,"SRAb":0.0561201,"SRB":0.0461849,"SRBb":0.0512918,"SRC":0.0457167,"SRCb":0.0575734,"SRVZBosoted":0.0818975,"SRVZResolved":0.0373819,"SRHZ":0.00776693}
kappa = {"SRA":0.064,"SRAb":0.064,"SRB":0.067,"SRBb":0.067,"SRC":0.038,"SRCb":0.038,"SRVZBoosted":0.05538,"SRVZResolved":0.06221,"SRHZ":0.045}

data_file = r.TFile(os.path.join(hist_prefix,"Data.root"))
templates_file = r.TFile(os.path.join(hist_prefix,"Templates.root"))



