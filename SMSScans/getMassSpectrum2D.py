#!/usr/bin/env python
import ROOT, sys, sets, os

mass_points = sets.Set()
files_in = []
output_filename = ""

def checkInputs():
  if (len(sys.argv) < 2) or (".root" in sys.argv[1]):
    print("Usage: ")
    print("./getMassSpectrum2D.py <sample_name> <path_to_baby_1> <path_to_baby_2> ... <path_to_baby_n>")
    exit(1)

  print("Getting mass spectrum for %d files" % len(sys.argv[2:]))

def fillMassSpectrumFromTChain(samplename):

  print "mass spectrum from TTrees"

  directory_prefix = "/nfs-7/userdata/bsathian/ZMET_babies/"

  dataset_directory_mapping ={

        "t5zz":{2016:["SMS-T5ZZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM","SMS-T5ZZ_mGluino-1850to2400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM"],
            2017:["SMS-T5ZZ_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM","SMS-T5ZZ_mGluino-1850to2400_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM"],
            2018:["SMS-T5ZZ_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1_MINIAODSIM","SMS-T5ZZ_mGluino-1850to2400_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1_MINIAODSIM"]},

        "tchiwz":{2016:["SMS-TChiWZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM"],
            2017:["SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM"],
            2018:["SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1_MINIAODSIM"]},

        "tchihz":{2016:["SMS-TChiHZ_HToBB_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM"],
            2017:["SMS-TChiHZ_HToBB_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM"],
            2018:["SMS-TChiHZ_HToBB_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1_MINIAODSIM"]},

        "tchizz":{2016:["SMS-TChiZZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUSummer16v3Fast_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM"],
            2017:["SMS-TChiZZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PUFall17Fast_94X_mc2017_realistic_v15-v1_MINIAODSIM"],
            2018:["SMS-TChiZZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-PUFall18Fast_102X_upgrade2018_realistic_v15-v1_MINIAODSIM"]},

    }

  ch = ROOT.TChain("t")
  samplename = samplename.lower()
  for year in [2016,2017,2018]:
    for directory in dataset_directory_mapping[samplename][year]:
      full_path = os.path.join(directory_prefix,directory+"_ZMET_babies","baby_*.root")
      ch.Add(full_path)


  ch.SetBranchStatus("*", 0)
  ch.SetBranchStatus("mass_LSP", 1)
  ch.SetBranchStatus("mass_gluino", 1)

  n_entries = ch.GetEntries()
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
      print("Processing entry %d of %d" % (j_entry, n_entries))

    if ((ch.mass_gluino,ch.mass_LSP) not in mass_points):
      mass_points.add((ch.mass_gluino,ch.mass_LSP))


  outfile = open(output_filename, 'w')

  print len(mass_points), "mass points present"
  print "Writing mass points cache file"

  for i in mass_points:
    outfile.write("mass_gluino: %f \t mass_LSP: %f \n" % (i[0], i[1]))

  outfile.close()

  return mass_points

def fillMassSpectrumFromCache(samplename):
  """parse the mass binning file with output_filename as it's filename and fill the mass points set"""

  output_filename = "SMSSCans/Spectra/mass_spectrum_{}.txt".format(samplename)
  mass_file = open(output_filename, 'r')
  for line in mass_file:
    a=line.split()
    mass_points.add((int(float(a[1])), int(float(a[3]))))
  print "mass spectrum obtained from cache"
  return mass_points

def fillMassSpectrum(samplename):
  """checks if the sample name already has a mass binning file made, if it does, parse it and fill the mass spectrum, otherwise read the files in a build the mass spectrum on the fly."""
  output_filename = "SMSScans/Spectra/mass_spectrum_{}.txt".format(samplename)
  if not os.path.isfile(output_filename):
    print "Cache file not present! Recomputing from TChain"
    mass_points = fillMassSpectrumFromTChain(samplename)
  else:
    print "Cache file present!"
    mass_points = fillMassSpectrumFromCache(samplename)

  return mass_points

def getMassSpectrum(name):
  """returns a list of pairs (mass_gluino, mass_LSP)"""

  print "Getting mass spectrum..."
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name

  mass_points = fillMassSpectrum(name)

  if type(mass_points) is not list:
      mass_points = list(mass_points)

  return mass_points


if __name__ == "__main__":
  checkInputs()
  name=sys.argv[1]
  files_in=sys.argv[2:]
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
  fillMassSpectrum()

  for i in mass_points:
      print("mass_gluino: %f \t mass_LSP: %f" % (i[0], i[1]))

