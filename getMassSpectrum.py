#!/usr/bin/env python
import ROOT, sys, sets, os

def checkInputs():
  if (len(sys.argv) < 2):
    print("You must give the location of the babies!")
    print("Usage: ")
    print("./getMassSpectrum.py <sample_name> <path_to_baby_1> <path_to_baby_2> ... <path_to_baby_n>")
    exit(1)

  print("Getting mass spectrum for %d files" % len(sys.argv[1:]))

def getMassSpectrum():
  ch = ROOT.TChain("t")
  for i in sys.argv[2:]:
    ch.Add(i)

  mass_points = sets.Set()
  n_entries = ch.GetEntries()
  for j_entry in range(n_entries):
    if j_entry > 100 == 0:
      break

    i_entry = ch.LoadTree(j_entry)
    if i_entry < 0:
      break
    nb = ch.GetEntry(j_entry)
    if nb <= 0:
      continue


    if j_entry % 10000 == 0:
      print("Processing entry %d of %d" % (j_entry, n_entries))

    if ((ch.mass_gluino,ch.mass_LSP) not in mass_points):
      mass_points.add((ch.mass_gluino,ch.mass_LSP))

  outfile = open(output_filename, 'w')

  for i in mass_points:
    print("mass gluino: %f \t mass_LSP: %f" % (i[0], i[1]))
    outfile.Write("mass gluino: %f \t mass_LSP: %f \n" % (i[0], i[1]))


checkInputs()
output_filename = "SMSscans/mass_spectrum_%s.txt" % sys.argv[1]

if not os.path.isfile(output_filename):
  getMassSpectrum()
else:
  mass_file = open(output_filename, 'r')
  for line in mass_file:
    print(line)
  exit();