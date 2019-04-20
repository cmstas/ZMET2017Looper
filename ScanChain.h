// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <utility>
#include <fstream>
#include <unordered_map>


// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TEfficiency.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

// Analysis Specific
#include "ZMET2016.h"

// CORE
//You can not include headers!!! This is not compiled code.
#include "External/dorky.h"
//#include "External/goodrun.cc"
#include "External/MT2Utility.h"
#include "External/MT2.h"
// Configuration parsing
#include "ConfigParser.h"
#include "ConfigHelper.h"
//Histogram Filling
#include "HistTools.h"

using namespace std;
//using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;



class ZMETLooper
{

    private:
    ConfigParser *conf;
    int nDuplicates;
    int num_events_veto_ttbar;
    int num_events_veto_ttgamma;
    bool MCTriggerEmulation;
 

    //master histogram map
    std::unordered_map<std::string,TH1*> allHistos; 

    vector<pair <TH1D*, TString> > g_reweight_pairs;
    TDirectory *rootdir;
    TH1D *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36; 

    //Btag and ISR Scale Factor overall normalization
    TH2D *g_btagsf_norm, *g_btagsf_light_norm_up, *g_btagsf_heavy_norm_up;
    TH2D *g_isr_norm, *g_isr_norm_up;
    TFile *g_SUSYsf_norm_file;

    TEfficiency *g_pt_eff_barrel, *g_pt_eff_endcap; 
    TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
    TString g_sample_name;
    TFile* currentFile;
    double g_scale_factor; //Holds scale factors for sample.

    TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 

    bool printStats;
    bool printFail;

    //=======================================================
    // Global variables used for uncertainty fluctuations
    //=======================================================
    double g_dphi_metj1;
    double g_dphi_metj2;
    int g_njets;
    double g_mbb;
    double g_mjj_mindphi;
    int g_nBJetMedium;
    double g_met;
    double g_met_phi;
    double g_mt2;
    double g_mt2b;
    double g_ht;

    vector<float> g_jets_csv;
    vector<LorentzVector> g_jets_p4;
    vector<LorentzVector> g_jets_medb_p4;

    const int Z_PDG_ID;
    const int W_PDG_ID;
    const int H_PDG_ID;

    const int Z_MASS;
    const int W_MASS;
    const int H_MASS;

    /* returns two most B-like jet indicies */
    pair<int, int> getMostBlike();

    /*Finds the pair of B tagged jets (csv medium) with dijet mass closest to the mass of the higgs*/
    pair<int,int> getClosestBPairToHiggsMass();

    /*Builds MT2b from two highest CSV jets*/
    double getMT2B();

    /*Builds Mbb from two highest CSV jets*/
    double getMbb();

    /*This function gets the MT2 built out of the two Bjets in an event, no guarentee 
    is made about selecting the highest csv jets*/
    double getMT2ForBjets(bool select_highest_csv=false);

    /*Builds MT2 for the two leading Bjets unless select_closest_higgs_mass is set, in which case it 
    builds it out of the two bjets with dijet mass nearest the mass of the higgs.*/
    double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false);

    /*Returns boson Pt, determines whether sample is gjets or zjets first*/
    double bosonPt();

    /* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
    double getMTLepMET(short id=0);

    /* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
    double getdRGammaLep(short id=0);

    /*Returns position in the gen particles for the ewk boson and first quark pair prodcued from the EWK Boson mother and SUSY grandmother. 

  The function looks for a pair of quarks next to each other in the gen record with a mother that's the proper boson and a SUSY grandma.
  Then it checks that their diquark mass is within 10 GeV of boson's resonance mass, if this is true, the location of the pair is noted. 

  Next it searches through the gen record backwards starting from the entry prior to the quarks position when they can be found, on cmd line it 
  looked like ewk boson was normally the previous entry) and finds a EWK boson of proper flavor. If that boson has pt within 10 GeV of the diquark 
  system, the boson's position is noted.

  If no boson can be found, -1 is returned as the first position. 
  If no quarks can be found, -1 is returned as the second positon.*/
    pair<int,int> getSUSYHadDecayBoson();

    /*Returns the DeltaR between objects p1 and p2.*/
    double DeltaR(const LorentzVector p1, const LorentzVector p2);
    //=============================
    // Triggers
    //=============================
    /*Checks that the event passes an "emulated photon trigger"*/

    bool passPhotonEmulatedTrigger();

    /*Ensures the event is within the efficiency plateu of the highest pt trigger it passed*/
    bool passPhotonTriggers();

    /*MC passes immediately, ensures data events were gathered from di-muon triggers*/
    bool passMuonTriggers();

    /*MC passes immediately, ensures data events were gathered from di-electron triggers*/
    bool passElectronTriggers();

    /*MC passes immediately, ensures data events were gathered from EMu triggers*/
    bool passEMuTriggers();

    /*MC passes immediately, ensures data events were gathered from SingleMu trigger*/
    bool passSingleMuTriggers();

    /*Helper method which chooses which above method to call. Calls EMu if the dil_flavor is emu, otherwise uses
    the hyp_type to determine which to call. Events fail if they are hyp_type 2 and not tagged for emu*/
    bool passLeptonHLTs();

    //=============================
    // Has Good Event Functions
    //=============================
    /*Lepton quality and Z mass cuts*/

    bool hasGoodZ(); //0 for ee, 1 for emu, 2 for mumu, -1 for no lepton
    int dil_flavor;

    /*Photon quality cuts*/
    bool hasGoodPhoton();

    /*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
    It just checks muon quality stuff and then calls hasGoodPhoton()*/
    bool hasGoodGammaMu();

    /*Just a helper method that chooses which hasGood method to call based on the config event_type*/
    bool hasGoodEvent();

    //=============================
    // Event Weight Assignment
    //=============================

    /*Goes through the chain of weight_from config options down to a config which does not have weight_from and
    then adds a pair (config_name, hist_file) to the vector g_reweight_pairs.

    For now this is depricated: NEEDS TO BE UPDATED WITH NEW CODE FIXES*/
    void readyReweightHists();

    /* Adds the vpt reweighting histogram to the g_reweight_pairs vector */
    void readyVPTReweight(TString save_path);

    /* Returns the trigger efficiency from g_pt_eff */
    double getEff(const double &pt, const double &eta);

    /*Loads the reweight hists from g_reweight_pairs and multiplies returns the weight associated with the proper
    bin in the histogram*/
    double getReweight();

    /*This method stores fixes to the evt_scale1fb in the event of file corruptions. 
    It's basically just a lookup table*/
    double scale1fbFix();

    /*Main function for determining the weights for each event*/
    double getWeight();

    /*Returns the weight associated with the photon prescales*/
    double getPrescaleWeight();

    //=============================
    // Cuts
    //=============================

    /*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
    bool passSignalRegionCuts();

    /*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
    over samples tagged as "rares". This is only neccesary for the full prediction.*/
    bool passRareCuts();

    /*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/
    bool passSUSYSingalCuts();

    /*Front end method to "Dorky" duplicate removal*/
    bool isDuplicate();

    /*Checks for MET filters*/
    bool passMETFilters();

    /*Holds baseline cuts*/
    bool passBaseCut();

    /*Ensures events from the ee/mumu/emu dataset pass the trigger for that type of event and for ee and emu ensures they don't pass other triggers.*/
    bool passETHDileptonDataCleanse();

    /*Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples*/
    bool passFileSelections();

    //=============================
    // Setup
    //=============================
    /*Sets up global variables for the event which are the quantities that might be fluctuated in the process of computing uncertainty limits*/
    void setupGlobals();

    /*Loads the proper TH2 for the given SUSY sample which contains the BTag and ISR weights if running SUSY MC.*/
    void updateSUSYBtagISRNorms();

    /*Loads Pt reweighting histograms, pileup reweighting hists, and efficiency hists (which are no longer used really). Also sets up goodrun list*/
    void setupExternal(TString savePath);

    //Photon HLT Test Histograms
    //void PhotonHLTTest();
    //void FillTriggerHistograms();


    //Histogram stuff
    const int n_weight_log_bins = 54;
    const double weight_log_bins[] = {-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -.9, -.8, -.7, -.6, -.5, -.4, -.3, -.2, -.1, -.09, -.08, -.07, -.06, -.05, -.04, -.03, -.02, -.01, 0, .01, .02, .03, .04, .05, .06, .07, .08, .09, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};
    
    const int n_metbins_wide_std = 6;
    const double metbins_wide_std[] = {0, 50, 100, 150, 225, 300, 500};

    const int n_ptbins_std = 10;
    const double ptbins_std[] = {0, 22, 33, 40, 55, 85, 105, 135, 180, 250, 6000};

    const int n_ptbins_fine = 51;
    const double ptbins_fine[] = {0, 22, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 300, 350, 400, 6001};
    
    
    const int n_gluino_bins_t5zznat = 24;
    const double gluino_bins_t5zznat[]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000};

    const int n_lsp_bins_t5zznat = 66;
    const double lsp_bins_t5zznat[]={25.000000,50.000000,100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,790.000000,800.000000,850.000000,890.000000,900.000000,950.000000,990.000000,1000.000000,1040.000000,1050.000000,1090.000000,1100.000000,1140.000000,1150.000000,1190.000000,1200.000000,1240.000000,1250.000000,1290.000000,1300.000000,1340.000000,1350.000000,1390.000000,1400.000000,1440.000000,1450.000000,1490.000000,1500.000000,1540.000000,1550.000000,1590.000000,1600.000000,1640.000000,1650.000000,1690.000000,1700.000000,1740.000000,1750.000000,1790.000000,1800.000000,1840.000000,1850.000000,1890.000000,1900.000000,1940.000000,1950.000000,1990.000000,2000.000000,2040.000000,2050.000000,2090.000000};

    const int n_met_bins_t5zznat = 5;
    const double met_bins_t5zznat[] = {50, 100, 150, 250, 350, 6000};

    //==============================
    //T5ZZ contrived binning 
    //==============================

    const int n_gluino_bins_t5zzcontrived = 24;
    const double gluino_bins_t5zzcontrived[]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000};

    const int n_lsp_bins_t5zzcontrived = 39;
    const double lsp_bins_t5zzcontrived[]={100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,800.000000,850.000000,900.000000,950.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000};

    const int n_met_bins_t5zzcontrived = 5;
    const double met_bins_t5zzcontrived[] = {50, 100, 150, 250, 350, 6000};
  
    //==============================
    //TChiWZ model natural binning
    //==============================
    const int n_gluino_bins_tchiwz = 24;
    const double gluino_bins_tchiwz[]={100.000000,125.000000,150.000000,175.000000,200.000000,225.000000,250.000000,275.000000,300.000000,325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000};

    const int n_lsp_bins_tchiwz = 69;
    const double lsp_bins_tchiwz[]={1.000000,5.000000,10.000000,15.000000,20.000000,25.000000,30.000000,35.000000,40.000000,45.000000,50.000000,55.000000,60.000000,65.000000,70.000000,75.000000,80.000000,85.000000,90.000000,93.000000,95.000000,100.000000,105.000000,110.000000,115.000000,118.000000,120.000000,125.000000,130.000000,135.000000,140.000000,143.000000,145.000000,150.000000,155.000000,160.000000,165.000000,168.000000,170.000000,175.000000,180.000000,185.000000,190.000000,193.000000,195.000000,200.000000,205.000000,210.000000,215.000000,218.000000,220.000000,225.000000,230.000000,235.000000,240.000000,243.000000,245.000000,250.000000,255.000000,260.000000,265.000000,268.000000,270.000000,275.000000,280.000000,285.000000,290.000000,293.000000,295.000000,300.000000};

    const int n_met_bins_tchiwz = 5;
    const double met_bins_tchiwz[] = {50, 100, 150, 250, 350, 6000};

    //==============================
    //TChiWZ EXT model natural binning
    //==============================
    const int n_gluino_bins_tchiwz_ext = 15;
    const double gluino_bins_tchiwz_ext[]={325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000};

    const int n_lsp_bins_tchiwz_ext = 12;
    const double lsp_bins_tchiwz_ext[]={300.000000,305.000000,310.000000,315.000000,318.000000,320.000000,325.000000,330.000000,335.000000,340.000000,343.000000,345.000000,350.000000};

    const int n_met_bins_tchiwz_ext = 5;
    const double met_bins_tchiwz_ext[] = {50, 100, 150, 250, 350, 6000};

    //==============================
    //TChiHZ model natural binning
    //==============================
    //!!!!!!!!!!!!!!!!!!
    // Be sure to change the lowest bin to 125 to match the tchizz model.
    //!!!!!!!!!!!!!!!!!!
    const int n_chi_bins_tchihz = 35;
    const double chi_bins_tchihz[] = {125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000};

    const int n_met_bins_tchihz = 5;
    const double met_bins_tchihz[] = {50, 100, 150, 250, 350, 6000};


    //==============================
    //TChiZZ model natural binning
    //==============================
    const int n_chi_bins_tchizz = 36;
    const double chi_bins_tchizz[] = {100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000};

    const int n_met_bins_tchizz = 5;
    const double met_bins_tchizz[] = {50, 100, 150, 250, 350, 6000};

   
    const int *n_gluino_bins, *n_lsp_bins, *n_met_bins;
    const double *gluino_bins, *lsp_bins, *met_bins;

    const int *n_chi_bins;  //CHECK THIS DUDE IF YOU ENCOUNTER A met_bins RELATED ISSUE!!!
    const double *chi_bins;

    double weight=1;
    double sumMETFilters;

    //misc stuff
    double bb_pt;
    double mt2_val_fromb;
    double mt2_val_hz = 0;

    //SUSY variables
    
    double ISR_norm,btag_norm,ISR_norm_up,btag_heavy_norm_up,btag_light_norm_up;
    double dphi_gm;

    void fillCommonHists(std::string prefix="");
    void fillPhotonCRHists(std::string prefix = "");
    void fillGammaMuCRHists(std::string prefix = "");
    void fillDileptonCRHists(std::string prefix = "");
    void fillClosureHists(std::string prefix = "");
    void fillSignalRegionHists(std::string prefix = "");
    //SR Hists comin' soon...
    //


    public:
    ZMETLooper();
    int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1); 
};

//Global Vars
//
