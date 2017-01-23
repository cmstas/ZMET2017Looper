#!/usr/bin/env python
import ROOT, sys, sets

if (len(sys.argv) < 1):
  print("You must give the location of the babies!")
  print("Usage: ")
  print("./getMassSpectrum.py <path_to_baby_1> <path_to_baby_2> ... <path_to_baby_n>")
  exit(1)

ch = ROOT.TChain("t")
for i in sys.argv[1:]:
  ch.Add(i)

mass_points = sets.Set()

for j_entry in range(ch.GetEntries()):
  i_entry = ch.LoadTree(j_entry)
  if i_entry < 0:
    break
  nb = ch.GetEntry(j_entry)
  if nb <= 0:
    continue

  mass_points.add((ch.mass_gluino,ch.mass_LSP))

for i in mass_points:
  print("mass gluino: %f \t mass_LSP: %f" % (i[0], i[1]))