#!/usr/bin/env python
import ROOT, sys, sets, os, time

mass_points = sets.Set()
files_in = []
output_filename = ""

def checkInputs():
  if (len(sys.argv) < 2) or (".root" in sys.argv[1]):
    print("Usage: ")
    print("./getMassSpectrum2DCMS3.py <sample_name> <path_to_CMS3_1> <path_to_CMS3_2> ... <path_to_CMS3_n>")
    exit(1)

  print("Getting mass spectrum for %d files" % len(sys.argv[2:]))

def fillMassSpectrumFromTChain():
  ch = ROOT.TChain("Events")
  for i in files_in:
    ch.Add(i)

  ch.SetBranchStatus("*", 0)
  ch.SetBranchStatus("floats_sParmMaker_sparmvalues_CMS3.obj", 1)

  n_entries = ch.GetEntries()
  last_time = time.time()
  this_time = time.time()
  for j_entry in range(n_entries):
    
    #if j_entry > 1000:
    #  break

    i_entry = ch.LoadTree(j_entry)
    if i_entry < 0:
      break
    nb = ch.GetEntry(j_entry)
    if nb <= 0:
      continue


    if j_entry % 10000 == 0:
      this_time=time.time()
      print("Processing entry %d of %d, eta: %f secs" % (j_entry, n_entries, (this_time - last_time)*((n_entries-j_entry)/10000) ))
      last_time=this_time

    if ((ch.sparm_values[0],ch.sparm_values[1]) not in mass_points):
      mass_points.add((ch.sparm_values[0],ch.sparm_values[1]))

  outfile = open(output_filename, 'w')

  for i in mass_points:
    outfile.write("mass_gluino: %f \t mass_LSP: %f \n" % (i[0], i[1]))

  outfile.close()

def fillMassSpectrumFromCache():
  """parse the mass binning file with output_filename as it's filename and fill the mass points set"""
  mass_file = open(output_filename, 'r')
  for line in mass_file:
    a=line.split()
    mass_points.add((int(float(a[1])), int(float(a[3]))))

def fillMassSpectrum():
  """checks if the sample name already has a mass binning file made, if it does, parse it and fill the mass spectrum, otherwise read the files in a build the mass spectrum on the fly."""
  if not os.path.isfile(output_filename):
    fillMassSpectrumFromTChain()
  else:
    fillMassSpectrumFromCache()

def getMassSpectrum(name, f_paths):
  """returns a list of pairs (mass_gluino, mass_LSP)"""
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
  files_in = f_paths
  
  fillMassSpectrum()

  return mass_points

if __name__ == "__main__":
  checkInputs()
  name=sys.argv[1]
  files_in=sys.argv[2:]
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
  fillMassSpectrum()

  for i in mass_points:
      print("mass_gluino: %f \t mass_LSP: %f" % (i[0], i[1]))

