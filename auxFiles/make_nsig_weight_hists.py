import ROOT as r
import sys,os


dobtagsfs = True

fastsim_sample = sys.argv[1].lower()
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



ch = {2016:r.TChain("t"),2017:r.TChain("t"),2018:r.TChain("t")}
h_nsig = {}
h_avg_weight_btagsf = {}
h_avg_weight_btagsf_heavy_UP = {}
h_avg_weight_btagsf_light_UP = {}
h_avg_weight_btagsf_heavy_DN = {}
h_avg_weight_btagsf_light_DN = {}

h_avg_weight_isr = {}
h_avg_weight_isr_DN = {}
h_avg_weight_isr_UP = {}

if fastsim_sample == "tchihz" or fastsim_sample == "tchizz":
    var = "mass_LSP:mass_gluino"
else:
    var = "mass_LSP:mass_chi"

for year in [2016,2017,2018]:
    for directory in dataset_directory_mapping[fastsim_sample][year]:
        full_path = os.path.join(directory_prefix,directory+"_ZMET_babies","baby_1.root")
        ch[year].Add(full_path)
    if fastsim_sample == "t5zz" or fastsim_sample == "tchiwz":
        h_nsig[year] = r.TH2D("h_nsig_"+str(year),";mass1 [GeV];mass2 [GeV]",60,0,300,60,0,300)
    elif fastsim_sample == "tchiwz" or fastsim_sample == "tchizz":
        h_nsig[year] = r.TH2D("h_nsig_"+str(year),";mass1 [GeV];mass2 [GeV]",60,0,300,1,-1000,1000)

    h_avg_weight_btagsf[year] = h_nsig[year].Clone("h_avg_weight_btagsf_"+str(year))
    h_avg_weight_btagsf_heavy_UP[year] = h_nsig[year].Clone("h_avg_weight_btagsf_heavy_UP_"+str(year));
    h_avg_weight_btagsf_light_UP[year] = h_nsig[year].Clone("h_avg_weight_btagsf_light_UP_"+str(year));
    h_avg_weight_btagsf_heavy_DN[year] = h_nsig[year].Clone("h_avg_weight_btagsf_heavy_DN_"+str(year));
    h_avg_weight_btagsf_light_DN[year] = h_nsig[year].Clone("h_avg_weight_btagsf_light_DN_"+str(year));
    h_avg_weight_isr[year] = h_nsig[year].Clone("h_avg_weight_isr_"+str(year))
    h_avg_weight_isr_UP[year] = h_nsig[year].Clone("h_avg_weight_isr_UP_"+str(year))
    h_avg_weight_isr_DN[year] = h_nsig[year].Clone("h_avg_weight_isr_DN_"+str(year))

    if dobtagsfs:
        ch[year].Project(var+">>h_nsig_"+str(year))
        ch[year].Project(var+">>h_avg_weight_btagsf_"+str(year),"(met_pt > -1)*weight_btagsf")
        ch[year].Project(var+">>h_avg_weight_btagsf_heavy_UP_"+str(year),"(met_pt > -1.)*weight_btagsf_heavy_UP")
	ch[year].Project(var+">>h_avg_weight_btagsf_light_UP_"+str(year),"(met_pt > -1.)*weight_btagsf_light_UP")
	ch[year].Projectvar+">>h_avg_weight_btagsf_heavy_DN_"+str(year),"(met_pt > -1.)*weight_btagsf_heavy_DN")
	ch[year].Project(var+">>h_avg_weight_btagsf_light_DN_"+str(year),"(met_pt > -1.)*weight_btagsf_light_DN")

	ch[year].Project(var+">>h_avg_weight_isr_"+str(year)   ,"(met_pt > -1.)*isr_weight")
	ch[year].Project(var+">>h_avg_weight_isr_DN_"+str(year),"(met_pt > -1.)*(isr_weight-isr_unc)")
	ch[year].Project(var+">>h_avg_weight_isr_UP_"+str(year),"(met_pt > -1.)*(isr_weight+isr_unc)")

    h_avg_weight_btagsf[year].Divide(h_nsig[year])
    h_avg_weight_btagsf_heavy_UP[year].Divide(h_nsig[year])
    h_avg_weight_btagsf_light_UP[year].Divide(h_nsig[year])
    h_avg_weight_btagsf_heavy_DN[year].Divide(h_nsig[year])
    h_avg_weight_btagsf_light_DN[year].Divide(h_nsig[year])
    h_avg_weight_isr[year].Divide(h_nsig[year])
    h_avg_weight_isr_DN[year].Divide(h_nsig[year])
    h_avg_weight_isr_UP[year].Divide(h_nsig[year])


fout = r.TFile("nsig_weights_"+fastsim_sample+".root","RECREATE")
fout.cd()

for year in [2016,2017,2018]:
    h_nsig[year].SetDirectory(fout)
    h_avg_weight_btagsf[year].SetDirectory(fout)
    h_avg_weight_btagsf[year].SetDirectory(fout)
    h_avg_weight_btagsf_heavy_UP[year].SetDirectory(fout)
    h_avg_weight_btagsf_light_UP[year].SetDirectory(fout)
    h_avg_weight_btagsf_heavy_DN[year].SetDirectory(fout)
    h_avg_weight_btagsf_light_DN[year].SetDirectory(fout)
    h_avg_weight_isr[year].SetDirectory(fout)
    h_avg_weight_isr_DN[year].SetDirectory(fout)
    h_avg_weight_isr_UP[year].SetDirectory(fout)

fout.Write()
fout.Close()







