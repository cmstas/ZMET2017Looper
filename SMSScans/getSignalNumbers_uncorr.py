import ROOT as r
from collections import defaultdict
import numpy as np

def findJESDeviation(yeild,JES_up,JES_dn):
    """Returns a list of the greatest deviation in a bin between CV yeild with JES_up and JES_dn"""
    uncs=[]
    for y,up,dn in zip(yeild, JES_up, JES_dn):
        if abs(up-y) > abs(dn-y):
            uncs.append(abs(up-y))
        else:
            uncs.append(abs(dn-y))

    return uncs



def combine_counts(hists):
    """ Combines counts from the year split histograms to create a single list. hists is a misnomer here """

    final_yield = []
    #Get the anchor
    key = list(hists.keys())[0]
    anchor_hist = hists[key] #for bin indexing
    for bin_id in range(len(anchor_hist)):
        temp = 0
        for year in hists.keys():
            temp += hists[year][bin_id]

        final_yield.append(temp)

    return final_yield


def combine_uncertainties(var_hists,cv_hists,nonzero_cv = False):

    unc_values = []

    key = list(cv_hists.keys())[0]
    anchor_hist = cv_hists[key] #for bin indexing

    for bin_id in range(len(anchor_hist)):
        temp = 0

        if var_hists:
            for year in cv_hists.keys():
                if nonzero_cv and cv_hists[year][bin_id] == 0:
                    continue
                else:
                    if type(var_hists) is list:
                        temp += findJESDeviation(cv_hists[year],var_hists[0][year],var_hists[1][year])[bin_id] ** 2
                    else:
                        temp += (var_hists[year][bin_id] - cv_hists[year][bin_id]) ** 2
        else:
            #stat uncs
            for year in cv_hists.keys():
                temp += cv_hists[year][bin_id] ** 2

        unc_values.append(np.sqrt(temp))
   
    return unc_values




def getSignalYields(SR,mass_gluino,mass_lsp,file_path = "t5zz.root"):
    """Reads all the histograms for the scan and returns yeilds and uncertainies for the model in the signal region. Returns: (yield, stat uncertainty, btag light unc, btag heavy unc, isr unc, JES unc, fast sim MET unc)
    In the case of a 1D scan, send mass_lsp=-1"""

    if mass_lsp==-1:
        mass_chi=mass_gluino

    bins=[]

    file_path_JES_up=file_path.replace("/CV/", "/JES_up/")
    file_path_JES_dn=file_path.replace("/CV/", "/JES_dn/")
    file_path_GenMET=file_path.replace("/CV/", "/GenMET/")


    if SR in ["SRA","SRAb","SRB","SRBb"]:
        bins = [(50,100),(100,150),(150,230),(230,300),(300,6001)]

    elif SR=="SRC" or SR=="SRCb":
        bins = [(50,100),(100,150),(150,250),(250,6001)]

    elif SR=="SRVZResolved":
        bins = [(50,100), (100,150), (150,250), (250, 350), (350, 6001)]

    elif SR == "SRVZBoosted":
        bins = [(50,100),(100,200),(200,300),(300,400),(400,500),(500,6001)]
    else:
        bins = [(50,100), (100,150), (150,250), (250, 6001)]



    RecoMET_yields = defaultdict(list)
    RecoMET_hists = {}
    GenMET_yields = defaultdict(list)
    GenMET_hists = {}
    avg_yields = defaultdict(list)

    stat_unc_bin = r.Double()

    stat_uncs = defaultdict(list)

    bl_yields = defaultdict(list)
    bl_hists = {}
    bh_yields = defaultdict(list)
    bh_hists = {}
    isr_yields = defaultdict(list)
    isr_hists = {}
    JES_up = defaultdict(list)
    JES_up_hists = {}
    JES_dn = defaultdict(list)
    JES_dn_hists = {}

    tau21_up = defaultdict(list)
    tau21_up_hists = {}

    tau21_down = defaultdict(list)
    tau21_down_hists = {}



   #Separate histograms for each year

    file_paths = {}
    file_paths_JES_up = {}
    file_paths_JES_dn = {}
    file_paths_GenMET = {}

    files = {}
    files_JES_up = {}
    files_JES_dn = {}
    files_GenMET = {}

    file_paths["2016"] = file_path.replace("combined","2016")
    file_paths["2017"] = file_path.replace("combined","2017")
    file_paths["2018"] = file_path.replace("combined","2018")

    for year,file_path in file_paths.items():
        file_paths_JES_up[year] = file_path.replace("CV","JES_up")
        file_paths_JES_dn[year] = file_path.replace("CV","JES_dn")
        file_paths_GenMET[year] = file_path.replace("CV","GenMET")
    

    for year in file_paths.keys():
        files[year] = r.TFile(file_paths[year])
        files_JES_up[year] = r.TFile(file_paths_JES_up[year])
        files_JES_dn[year] = r.TFile(file_paths_JES_dn[year])
        files_GenMET[year] = r.TFile(file_paths_GenMET[year])

    for year in file_paths.keys():
        RecoMET_hists[year] = files[year].Get(SR+"susy_type1MET_counts").Clone("count"+year)
        bl_hists[year] = files[year].Get(SR+"susy_type1MET_btaglight_up").Clone("btag_light_up"+year)
        bh_hists[year] = files[year].Get(SR+"susy_type1MET_btagheavy_up").Clone("btag_heavy_up"+year)
        isr_hists[year] = files[year].Get(SR+"susy_type1MET_isr_up").Clone("isr_up"+year)
        JES_up_hists[year] = files_JES_up[year].Get(SR+"susy_type1MET_counts").Clone("JES_up"+year)
        JES_dn_hists[year] = files_JES_dn[year].Get(SR+"susy_type1MET_counts").Clone("JES_dn"+year)
        GenMET_hists[year] = files_GenMET[year].Get(SR+"susy_type1MET_counts").Clone("GenMET"+year)

        if "Boosted" in SR:
            tau21_up_hists[year] = files[year].Get(SR+"susy_type1MET_tau21_up").Clone("tau21_up"+year)
            tau21_down_hists[year] = files[year].Get(SR+"susy_type1MET_tau21_down").Clone("tau21_down"+year)


#Add the errors in quadrature, the counts in sums

    for year in file_paths.keys():

        #Very stupid hack
        count = RecoMET_hists[year]
        GenMET_count = GenMET_hists[year]
        btag_light_up = bl_hists[year]
        btag_heavy_up = bh_hists[year]
        JES_up_count = JES_up_hists[year]
        JES_dn_count = JES_dn_hists[year]
        isr_up = isr_hists[year]

        if "Boosted" in SR:
            tau21_up_count = tau21_up_hists[year]
            tau21_down_count = tau21_down_hists[year]

        if mass_lsp == -1:
            for b in bins:
                RecoMET_yields[year].append(count.IntegralAndError(count.GetXaxis().FindBin(b[0]),
                                           count.GetXaxis().FindBin(b[1] - 0.001),
                                           count.GetYaxis().FindBin(mass_chi),
                                           count.GetYaxis().FindBin(mass_chi),
                                           stat_unc_bin))
                stat_uncs[year].append(float(stat_unc_bin))

                GenMET_yields[year].append(GenMET_count.Integral(GenMET_count.GetXaxis().FindBin(b[0]),
                                           GenMET_count.GetXaxis().FindBin(b[1] - 0.001),
                                           GenMET_count.GetYaxis().FindBin(mass_chi),
                                           GenMET_count.GetYaxis().FindBin(mass_chi)))

                avg_yields[year].append((GenMET_yields[year][-1] + RecoMET_yields[year][-1])/2.0)

                bl_yields[year].append(btag_light_up.Integral(btag_light_up.GetXaxis().FindBin(b[0]),
                                              btag_light_up.GetXaxis().FindBin(b[1] - 0.001),
                                              btag_light_up.GetYaxis().FindBin(mass_chi),
                                              btag_light_up.GetYaxis().FindBin(mass_chi)))

                bh_yields[year].append(btag_heavy_up.Integral(btag_heavy_up.GetXaxis().FindBin(b[0]),
                                              btag_heavy_up.GetXaxis().FindBin(b[1] - 0.001),
                                              btag_heavy_up.GetYaxis().FindBin(mass_chi),
                                              btag_heavy_up.GetYaxis().FindBin(mass_chi)))

                isr_yields[year].append(isr_up.Integral(isr_up.GetXaxis().FindBin(b[0]),
                                        isr_up.GetXaxis().FindBin(b[1] - 0.001),
                                        isr_up.GetYaxis().FindBin(mass_chi),
                                        isr_up.GetYaxis().FindBin(mass_chi)))

                JES_up[year].append(JES_up_count.Integral(JES_up_count.GetXaxis().FindBin(b[0]),
                                           JES_up_count.GetXaxis().FindBin(b[1] - 0.001),
                                           JES_up_count.GetYaxis().FindBin(mass_chi),
                                           JES_up_count.GetYaxis().FindBin(mass_chi)))

                JES_dn[year].append(JES_dn_count.Integral(JES_dn_count.GetXaxis().FindBin(b[0]),
                                           JES_dn_count.GetXaxis().FindBin(b[1] - 0.001),
                                           JES_dn_count.GetYaxis().FindBin(mass_chi),
                                           JES_dn_count.GetYaxis().FindBin(mass_chi)))

                if "Boosted" in SR:
                    tau21_up[year].append(tau21_up_count.Integral(tau21_up_count.GetXaxis().FindBin(b[0]),
                                            tau21_up_count.GetXaxis().FindBin(b[1] - 0.001),
                                            tau21_up_count.GetYaxis().FindBin(mass_chi),
                                            tau21_up_count.GetYaxis().FindBin(mass_chi)))

                    tau21_down[year].append(tau21_down_count.Integral(tau21_down_count.GetXaxis().FindBin(b[0]),
                        tau21_down_count.GetXaxis().FindBin(b[1] - 0.001),
                        tau21_down_count.GetYaxis().FindBin(mass_chi),
                        tau21_down_count.GetYaxis().FindBin(mass_chi)))


        else:
            for b in bins:
                RecoMET_yields[year].append(count.IntegralAndError(count.GetXaxis().FindBin(b[0]),
                                           count.GetXaxis().FindBin(b[1] - 0.001),
                                           count.GetYaxis().FindBin(mass_gluino),
                                           count.GetYaxis().FindBin(mass_gluino),
                                           count.GetZaxis().FindBin(mass_lsp),
                                           count.GetZaxis().FindBin(mass_lsp),
                                           stat_unc_bin))
                stat_uncs[year].append(float(stat_unc_bin))

                GenMET_yields[year].append(GenMET_count.Integral(GenMET_count.GetXaxis().FindBin(b[0]),
                                           GenMET_count.GetXaxis().FindBin(b[1] - 0.001),
                                           GenMET_count.GetYaxis().FindBin(mass_gluino),
                                           GenMET_count.GetYaxis().FindBin(mass_gluino),
                                           GenMET_count.GetZaxis().FindBin(mass_lsp),
                                           GenMET_count.GetZaxis().FindBin(mass_lsp)))

                avg_yields[year].append((GenMET_yields[year][-1] + RecoMET_yields[year][-1])/2.0)

                bl_yields[year].append(btag_light_up.Integral(btag_light_up.GetXaxis().FindBin(b[0]),
                                              btag_light_up.GetXaxis().FindBin(b[1] - 0.001),
                                              btag_light_up.GetYaxis().FindBin(mass_gluino),
                                              btag_light_up.GetYaxis().FindBin(mass_gluino),
                                              btag_light_up.GetZaxis().FindBin(mass_lsp),
                                              btag_light_up.GetZaxis().FindBin(mass_lsp)))

                bh_yields[year].append(btag_heavy_up.Integral(btag_heavy_up.GetXaxis().FindBin(b[0]),
                                              btag_heavy_up.GetXaxis().FindBin(b[1] - 0.001),
                                              btag_heavy_up.GetYaxis().FindBin(mass_gluino),
                                              btag_heavy_up.GetYaxis().FindBin(mass_gluino),
                                              btag_heavy_up.GetZaxis().FindBin(mass_lsp),
                                              btag_heavy_up.GetZaxis().FindBin(mass_lsp)))

                isr_yields[year].append(isr_up.Integral(isr_up.GetXaxis().FindBin(b[0]),
                                        isr_up.GetXaxis().FindBin(b[1] - 0.001),
                                        isr_up.GetYaxis().FindBin(mass_gluino),
                                        isr_up.GetYaxis().FindBin(mass_gluino),
                                        isr_up.GetZaxis().FindBin(mass_lsp),
                                        isr_up.GetZaxis().FindBin(mass_lsp)))

                JES_up[year].append(JES_up_count.Integral(JES_up_count.GetXaxis().FindBin(b[0]),
                                           JES_up_count.GetXaxis().FindBin(b[1] - 0.001),
                                           JES_up_count.GetYaxis().FindBin(mass_gluino),
                                           JES_up_count.GetYaxis().FindBin(mass_gluino),
                                           JES_up_count.GetZaxis().FindBin(mass_lsp),
                                           JES_up_count.GetZaxis().FindBin(mass_lsp)))

                JES_dn[year].append(JES_dn_count.Integral(JES_dn_count.GetXaxis().FindBin(b[0]),
                                           JES_dn_count.GetXaxis().FindBin(b[1] - 0.001),
                                           JES_dn_count.GetYaxis().FindBin(mass_gluino),
                                           JES_dn_count.GetYaxis().FindBin(mass_gluino),
                                           JES_dn_count.GetZaxis().FindBin(mass_lsp),
                                           JES_dn_count.GetZaxis().FindBin(mass_lsp)))
                if "Boosted" in SR:
                    tau21_up[year].append(tau21_up_count.Integral(tau21_up_count.GetXaxis().FindBin(b[0]),
                                           tau21_up_count.GetXaxis().FindBin(b[1] - 0.001),
                                           tau21_up_count.GetYaxis().FindBin(mass_gluino),
                                           tau21_up_count.GetYaxis().FindBin(mass_gluino),
                                           tau21_up_count.GetZaxis().FindBin(mass_lsp),
                                           tau21_up_count.GetZaxis().FindBin(mass_lsp)))

                    tau21_down[year].append(tau21_down_count.Integral(tau21_down_count.GetXaxis().FindBin(b[0]),
                                           tau21_down_count.GetXaxis().FindBin(b[1] - 0.001),
                                           tau21_down_count.GetYaxis().FindBin(mass_gluino),
                                           tau21_down_count.GetYaxis().FindBin(mass_gluino),
                                           tau21_down_count.GetZaxis().FindBin(mass_lsp),
                                           tau21_down_count.GetZaxis().FindBin(mass_lsp)))





    #Compute the deviations right here and return the errors computed

    RecoMET_yield_combined = []
    GenMET_yield_combined = []
    average_yield_combined = []
    stat_unc_combined = []
    MET_nuisance_combined = []
    bl_nuisance_combined = []
    bh_nuisance_combined = []
    isr_nuisance_combined = []
    JES_nuisance_combined = []

    if "Boosted" in SR:
        tau21_up_nuisance_combined = []
        tau21_down_nuisance_combined = []


    RecoMET_yield_combined = combine_counts(RecoMET_yields)
    GenMET_yield_combined = combine_counts(GenMET_yields)
    avg_yield_combined = combine_counts(avg_yields)

    stat_unc_combined = combine_uncertainties(cv_hists = stat_uncs,var_hists = None)
    MET_nuisance_combined = combine_uncertainties(RecoMET_yields,avg_yields,nonzero_cv = True)
    bl_nuisance_combined = combine_uncertainties(bl_yields,RecoMET_yields)
    isr_nuisance_combined = combine_uncertainties(isr_yields,RecoMET_yields)
    bh_nuisance_combined = combine_uncertainties(bh_yields,RecoMET_yields)
    JES_nuisance_combined = combine_uncertainties([JES_up,JES_dn],RecoMET_yields) #Special snowflake

    if "Boosted" in SR:
        tau21_up_nuisance_combined = combine_uncertainties(tau21_up,RecoMET_yields)
        tau21_down_nuisance_combined = combine_uncertainties(tau21_down,RecoMET_yields)

    if "Boosted" in SR:
        return (avg_yield_combined, RecoMET_yield_combined, stat_unc_combined,bl_nuisance_combined,bh_nuisance_combined,MET_nuisance_combined,isr_nuisance_combined,JES_nuisance_combined,tau21_up_nuisance_combined,tau21_down_nuisance_combined)

    else:
        return (avg_yield_combined, RecoMET_yield_combined, stat_unc_combined,bl_nuisance_combined,bh_nuisance_combined,MET_nuisance_combined,isr_nuisance_combined,JES_nuisance_combined)

