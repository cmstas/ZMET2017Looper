#!/usr/bin/env python
from __future__ import print_function
import argparse, sys, re
import sys,os
import numpy as np
import json

def properSpacing(key, param):
  """return param padded with spaces so that it's length {key} to preserve tabbing in templates"""

  delta=len(key)-len(param)

  if (len(param) < len(key)):
    param=(" "*delta)+param

  return param


def getNuisenceParameters(SR):
    """Reads in the output of the plot maker for the signal region and collects all the key value pairs of nuisance parameters."""
    print("SR=",SR)
    f = open("/home/users/bsathian/ZMet/histsthreeyears/strategy_B/loose_BVeto/DoubleLepton/combined/statsplots_{0}.out".format(SR),"r")
    n_dict = {}
    for line in f:
        if re.match("{.*} [0-9]*\.[0-9]*\s$", line):
            toks=line.split()
            if "zjets" in toks[0]:
                n_dict[toks[0][1:-1]] =  float(toks[1])

    return n_dict

def makeJSON(SR):
    nbins = {"SRA":5,"SRAb":5,"SRB":5,"SRBb":5,"SRC":4,"SRCb":4,"SRVZBoosted":6,"SRVZResolved":5,"SRHZ":4}
    if SR not in nbins.keys():
        print("Wrong SR!")
        sys.exit(1)
    n_parms = getNuisenceParameters(SR)
        #Do the appropriate multiplication
    output_params = {}
    for count in range(nbins[SR]):
        prediction = float(n_parms["BGbin{0}_zjets".format(count)])
        norm_error = (float(n_parms["zjets_norm"]) - 1 ) * prediction
        clos_error = (float(n_parms["zjets_clos_bin{0}".format(count)]) - 1) * prediction
        stat_error = (float(n_parms["zjets_stat_bin{0}".format(count)]) - 1) * prediction
        ewk_error = (float(n_parms["zjets_ewk_bin{0}".format(count)]) - 1) * prediction

        output_params["prediction_bin{0}".format(count)] = prediction
        output_params["norm_error_bin{0}".format(count)] = norm_error
        output_params["closure_error_bin{0}".format(count)] = clos_error
        output_params["stat_error_bin{0}".format(count)] = stat_error
        output_params["ewk_error_bin{0}".format(count)] = ewk_error

    with open("template_{0}.json".format(SR) ,"w") as f_out:
        json.dump(output_params,f_out)



def main():
    SR = sys.argv[1]
    makeJSON(SR)

if __name__ == "__main__":
    main()
