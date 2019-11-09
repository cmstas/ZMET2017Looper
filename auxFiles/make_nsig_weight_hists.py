from __future__ import print_function
import ROOT as r
import sys,os
import numpy as np

dobtagsfs = True

fastsim_sample = sys.argv[1].lower()
directory_prefix = "/nfs-7/userdata/bsathian/ZMET_babies_20191106/"

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

if fastsim_sample == "t5zz" or fastsim_sample == "tchiwz":
    var = "mass_LSP:mass_gluino"
else:
    var = "mass_LSP:mass_chi"


gluinoMassBins = np.array([800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000,2150.000000,2200.000000,2250.000000,2300.000000,2350.000000,2400.000000], dtype = np.float64)

lspMassBins = np.array([25.000000,50.000000,100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,790.000000,800.000000,850.000000,890.000000,900.000000,950.000000,990.000000,1000.000000,1040.000000,1050.000000,1090.000000,1100.000000,1140.000000,1150.000000,1190.000000,1200.000000,1240.000000,1250.000000,1290.000000,1300.000000,1340.000000,1350.000000,1390.000000,1400.000000,1440.000000,1450.000000,1490.000000,1500.000000,1540.000000,1550.000000,1590.000000,1600.000000,1640.000000,1650.000000,1690.000000,1700.000000,1740.000000,1750.000000,1790.000000,1800.000000,1840.000000,1850.000000,1890.000000,1900.000000,1940.000000,1950.000000,1990.000000,2000.000000,2040.000000,2050.000000,2090.000000,2100.000000,2140.000000,2150.000000,2190.000000,2200.000000,2240.000000,2250.000000,2290.000000,2300.000000,2340.000000,2350.000000,2390.000000,2400.000000], dtype = np.float64)

tchiwz_gluinoMassBins = np.array([100.000000,125.000000,150.000000,175.000000,200.000000,225.000000,250.000000,275.000000,300.000000,325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000,725.000000,750.000000,775.000000,800.000000,825.000000,850.000000,875.000000,900.00000,925.000000,950.000000,975.000000,1000.000000,1025.000000,1050.000000],dtype = np.float64)

tchiwz_lspMassBins = np.array([1.000000,5.000000,10.000000,15.000000,20.000000,25.000000,30.000000,35.000000,40.000000,45.000000,50.000000,55.000000,60.000000,65.000000,70.000000,75.000000,80.000000,85.000000,90.000000,93.000000,95.000000,100.000000,105.000000,110.000000,115.000000,118.000000,120.000000,125.000000,130.000000,135.000000,140.000000,143.000000,145.000000,150.000000,155.000000,160.000000,165.000000,168.000000,170.000000,175.000000,180.000000,185.000000,190.000000,193.000000,195.000000,200.000000,205.000000,210.000000,215.000000,218.000000,220.000000,225.000000,230.000000,235.000000,240.000000,243.000000,245.000000,250.000000,255.000000,260.000000,265.000000,268.000000,270.000000,275.000000,280.000000,285.000000,290.000000,293.000000,295.000000,300.000000,305.000000,310.000000,315.000000,318.000000,320.000000,325.000000,330.000000,335.000000,340.000000,343.000000,345.000000,350.000000,355.000000,360.000000,365.000000,368.000000,370.000000,375.000000,380.000000,385.000000,390.000000,393.000000,395.000000,400.000000,405.000000,410.000000,415.000000,418.000000,420.000000,425.000000,430.000000,435.000000,440.000000,443.000000,445.000000,450.000000,455.000000,460.000000,465.000000,468.000000,470.000000,475.000000,480.000000,485.000000,490.000000,493.000000,495.000000,500.000000,505.000000,510.000000,515.000000,518.000000,520.000000,525.000000,530.000000,535.000000,540.000000,543.000000,545.000000,550.000000,555.000000,560.000000,565.000000,568.000000,570.000000,575.000000,580.000000,585.000000,590.000000,593.000000,595.000000,600.000000,610.000000],dtype = np.float64)



for year in [2016,2017,2018]:
    print("Making histograms for year=",year)
    for directory in dataset_directory_mapping[fastsim_sample][year]:
        full_path = os.path.join(directory_prefix,directory+"_ZMET_babies","baby_*.root")
        print("Adding folder ",full_path)
        ch[year].Add(full_path)
    if fastsim_sample == "t5zz":
        h_nsig[year] = r.TH2D("h_nsig_"+str(year),";mass1 [GeV];mass2 [GeV]",len(gluinoMassBins)-1,gluinoMassBins,len(lspMassBins)-1,lspMassBins)
    elif fastsim_sample == "tchiwz" or fastsim_sample == "tchiwz-fullsim":
        h_nsig[year] = r.TH2D("h_nsig_"+str(year),";mass1 [GeV];mass2 [GeV]",len(tchiwz_gluinoMassBins)-1,tchiwz_gluinoMassBins,len(tchiwz_lspMassBins)-1,tchiwz_lspMassBins)

    elif fastsim_sample == "tchihz" or fastsim_sample == "tchizz":
        h_nsig[year] = r.TH2D("h_nsig_"+str(year),";mass1 [GeV];mass2 [GeV]",60,0,3000,1,-1000,1000)

    h_avg_weight_btagsf[year] = h_nsig[year].Clone("h_avg_weight_btagsf_"+str(year))
    h_avg_weight_btagsf_heavy_UP[year] = h_nsig[year].Clone("h_avg_weight_btagsf_heavy_UP_"+str(year));
    h_avg_weight_btagsf_light_UP[year] = h_nsig[year].Clone("h_avg_weight_btagsf_light_UP_"+str(year));
    h_avg_weight_btagsf_heavy_DN[year] = h_nsig[year].Clone("h_avg_weight_btagsf_heavy_DN_"+str(year));
    h_avg_weight_btagsf_light_DN[year] = h_nsig[year].Clone("h_avg_weight_btagsf_light_DN_"+str(year));
    h_avg_weight_isr[year] = h_nsig[year].Clone("h_avg_weight_isr_"+str(year))
    h_avg_weight_isr_UP[year] = h_nsig[year].Clone("h_avg_weight_isr_UP_"+str(year))
    h_avg_weight_isr_DN[year] = h_nsig[year].Clone("h_avg_weight_isr_DN_"+str(year))

    if dobtagsfs:
        ch[year].Project("h_nsig_"+str(year),var)
        ch[year].Project("h_avg_weight_btagsf_"+str(year),var,"(met_pt > -1)*weight_btagsf")
        ch[year].Project("h_avg_weight_btagsf_heavy_UP_"+str(year),var,"(met_pt > -1.)*weight_btagsf_heavy_UP")
	ch[year].Project("h_avg_weight_btagsf_light_UP_"+str(year),var,"(met_pt > -1.)*weight_btagsf_light_UP")
	ch[year].Project("h_avg_weight_btagsf_heavy_DN_"+str(year),var,"(met_pt > -1.)*weight_btagsf_heavy_DN")
	ch[year].Project("h_avg_weight_btagsf_light_DN_"+str(year),var,"(met_pt > -1.)*weight_btagsf_light_DN")

	ch[year].Project("h_avg_weight_isr_"+str(year),var,"(met_pt > -1.)*isr_weight")
	ch[year].Project("h_avg_weight_isr_DN_"+str(year),var,"(met_pt > -1.)*(isr_weight-isr_unc)")
	ch[year].Project("h_avg_weight_isr_UP_"+str(year),var,"(met_pt > -1.)*(isr_weight+isr_unc)")

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







