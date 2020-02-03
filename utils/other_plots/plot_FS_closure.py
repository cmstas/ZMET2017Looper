import ROOT as r
import plottery.plottery as ply
from root_numpy import hist2array
import numpy as np
import sys,os


def getFSError(bin_count,norm_up,norm_down,pt_up,pt_down,eta_up,eta_down,kappa,kappa_stat_error):
    kappa_MET_unc = 0.0134/0.0643;
    kappa_stat_unc = kappa_stat_error/kappa
    error_up = []
    error_down = []

    for i,count in bin_count:
        bin_up = r.Double(0.0)
        bin_down = r.Double(0.0)
        r.RooHistError.instance.getPoissonInterval(bin_count,bin_down, bin_up)
        rsfof_norm_unc = max(norm_up[i] - count, count - norm_down[i])
        rsfof_pt_unc = max(pt_up[i] - count, count - pt_down[i])
        rsfof_eta_unc = max(eta_up[i] - count, count - eta_down[i])

        bin_up = kappa*kappa*((bin_up - count)**2 + (kappa_stat_unc*count)**2 +  (kappa_MET_unc*count)**2 + rsfof_norm_unc**2 + rsfof_pt_unc**2 + rsfof_eta_unc**2)

        bin_down = kappa*kappa*((count-bin_down)**2 + (kappa_stat_unc*count)**2 + (kappa_MET_unc*count)**2 + rsfof_norm_unc**2 + rsfof_pt_unc**2 + rsfof_eta_unc**2)

        error_up.append(np.sqrt(bin_up))
        error_down.append(np.sqrt(bin_down))

    return (error_up,error_down)


FS_file = r.TFile("/home/users/bsathian/ZMet/histsthreeyears/Final/DoubleLepton/combined/TTJets.root")

prefix_path = "/home/users/bsathian/public_html/ZMET_plots/threeyears/FSClosure/"

strong_SR_binning = np.array([50,100,150,230,300,600],dtype = np.float64)
SRC_binning = np.array([50,100,150,250,600],dtype = np.float64)
SRVZBoosted_binning = np.array([50,100,200,300,400,500,600],dtype = np.float64)
SRVZResolved_binning = np.array([50,100,150,250,350,600],dype = np.float64)
SRHZ_binning = np.array([50,100,150,250,600],dtype = np.float64)

kappa_values = {
        "SRA":[0.064,0.003],
        "SRB":[0.067,0.005],
        "SRC":[0.038,0.018],
        "SRVZBoosted":[0.055,0.007],
        "SRVZResolved":[0.062,0.010],
        "SRHZ":[0.045,0.012],
        }

for SR in ["SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZBoosted","SRVZResolved","SRHZ"]:
    data_ee_hist = FS_file.Get(SR+"ee_type1MET").Clone("ee_type1MET")
    data_mumu_hist = FS_file.Get(SR+"mumu_type1MET").Clone("mumu_type1MET")
    data_hist = data_ee_hist.Clone(SR+"data_type1MET")
    data_hist.Add(data_mumu_hist)
    if "SRVZBoosted" in SR:
        binning = SRVZBoosted_binning
    elif "SRVZResolved" in SR:
        binning = SRVZResolved_binning
    elif "SRHZ" in SR:
        binning = SRHZ_binning
    elif "SRC" in SR:
        binning = SRC_binning
    else:
        binning =strong_SR_binning
    data_hist = data_hist.Rebin(len(binning),SR+"data_type1MET_rebinned",binning)

    mc_hist = FS_file.Get(SR+"emu_FS_type1MET").Clone(SR+"emu_type1MET")
    mc_hist = mc_hist.Rebin(len(binning)-1,SR+"mc_met_rebinned",binning)

    mc_hist_norm_up = FS_file.Get(SR+"emu_FS_type1MET_norm_up").Clone(SR+"emu_type1MET_norm_up").Rebin(len(binning)-1,SR+"mc_met_norm_up",binning)
    mc_hist_norm_down = FS_file.Get(SR+"emu_FS_type1MET_norm_down").Clone(SR+"emu_type1MET_norm_down").Rebin(len(binning)-1,SR+"mc_met_norm_down",binning)
    mc_hist_pt_up = FS_file.Get(SR+"emu_FS_type1MET_pt_up").Clone(SR+"emu_type1MET_pt_up").Rebin(len(binning)-1,SR+"mc_met_pt_up",binning)
    mc_hist_pt_down = FS_file.Get(SR+"emu_FS_type1MET_pt_down").Clone(SR+"emu_type1MET_pt_down").Rebin(len(binning)-1,SR+"mc_met_pt_down",binning)
    mc_hist_eta_up = FS_file.Get(SR+"emu_FS_type1MET_eta_up").Clone(SR+"emu_type1MET_eta_up").Rebin(len(binning)-1,SR+"mc_met_eta_up",binning)
    mc_hist_eta_down = FS_file.Get(SR+"emu_FS_type1MET_eta_down").Clone(SR+"emu_type1MET_eta_down").Rebin(len(binning)-1,SR+"mc_met_eta_down",binning)

    if SR[-1] == "b":
        kappa = kappa_values[SR[:-1]]
    else:
        kappa = kappa_values[SR]
    mc_hist.Scale(kappa)
    syst_hist = r.TH1F(SR+"syst_hist","",len(binning)-1,binning)
    bin_count = hist2array(mc_hist)
    norm_up = hist2array(mc_hist_norm_up)
    norm_down = hist2array(mc_hist_norm_down)
    pt_up = hist2array(mc_hist_pt_up)
    pt_down = hist2array(mc_hist_pt_down)
    eta_up = hist2array(mc_hist_eta_up)
    eta_down = hist2array(mc_hist_eta_down)

    FS_error_up, FS_error_down = getFSError(bin_count, norm_up, norm_down, pt_up, pt_down, eta_up, eta_down, kappa_kappa_stat_error)
    for i in range(1,syst_hist.NbinsX()+1):
        syst_hist.SetBinContent(FS_err_down[i-1] - FS_error_up[i-1])
    ply.plot_hist(
            data = data_hist,
            mc = [mc_hist],
            syst = syst_hist,
            legend_labels = ["MC e#mu"],
            options = {
                "yaxis_log":False,
                "yaxis_range":[1e-3,100],
                "legend_percentageinbox":False,
                "output_name":os.path.join(prefix_path,SR+"FSClosure.pdf")
                }
            )
