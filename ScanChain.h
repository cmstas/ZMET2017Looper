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
#include <cstdlib>
#include <algorithm>

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
#include "TVector2.h"
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

//reading json files
#include "readrMuEjson.h"

using namespace std;
//using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;



class ZMETLooper
{

    private:
    ConfigParser *conf;
    int nDuplicates;
    int year_fromCommandLine;
    int num_events_veto_ttbar;
    int num_events_veto_ttgamma;
    bool MCTriggerEmulation;

    int ee_eventCount = 0, mumu_eventCount = 0, emu_eventCount = 0;


    //master histogram map
    std::unordered_map<std::string,TH1*> allHistos;
    std::unordered_map<std::string,TH2*> all2DHistos;

    //Signal histograms
    std::unordered_map<std::string,TH2*> allSignal2DHistos;
    std::unordered_map<std::string,TH3*> allSignal3DHistos;

    vector<pair <TH1D*, TString> > g_reweight_pairs;
    //This is used only for multiple SR vpt reweighting
    std::unordered_map<std::string,TH1D*> g_vpt_reweight_pairs;

    TDirectory *rootdir;
    TH1D *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36;

    //Btag and ISR Scale Factor overall normalization
    TH2D *g_btagsf_norm, *g_btagsf_light_norm_up, *g_btagsf_heavy_norm_up;
    TH2D *g_isr_norm, *g_isr_norm_up;
    TFile *g_SUSYsf_norm_file;

    //2D Photon veto histogram based on ECAL bad spots

    TEfficiency *g_pt_eff_barrel, *g_pt_eff_endcap;
    TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
    TString g_sample_name;
    TFile* currentFile;
    double g_scale_factor; //Holds scale factors for sample.
    int g_year;

    TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts
    TH1I *ee_numEvents,*mumu_numEvents,*emu_numEvents;

    bool printStats;
    bool printFail;

    //=======================================================
    // Global variables used for uncertainty fluctuations
    //=======================================================
    double g_dphi_metj1;
    double g_dphi_metj2;
    double g_dphi_met_fatjet;
    int g_njets;
    double g_mbb;
    double g_mjj_mindphi;
    int g_nBJetMedium35;
    int g_nBJetMedium25;
    int g_nBJetLoose35;
    int g_nBJetLoose25;
    double g_met;
    double g_met_phi;
    double g_mt2;
    double g_mt2b;
    double g_ht;
    double g_mht;
    double g_mht_phi;
    double mhtMETDifference;

    vector<float> g_jets_csv;
    vector<LorentzVector> g_jets_p4;
    vector<LorentzVector> g_jets_medb_p4;
    //Need only for inclusive fat jet regions, but we fill this for everyone because Balaji is not a very smart dude
    //and makes mistakes, and so we need to help him debug SRs and VRs for tau21 and softdrop mass
    vector<size_t> g_fatjet_indices; //for the signal
    vector<size_t> g_fatjet_inclusive_indices;
    vector<size_t> g_fatjet_validation_indices;

    const int Z_PDG_ID = 23;
    const int W_PDG_ID = 24;
    const int H_PDG_ID = 25;

    const int Z_MASS = 91;
    const int W_MASS = 80;
    const int H_MASS = 125;

    double z_pt;


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

    int hasGoodZ(); //0 for ee, 1 for emu, 2 for mumu, -1 for no lepton
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
    void readyVPTReweight_allSR(TString save_path);

    /* Returns the trigger efficiency from g_pt_eff */
    double getEff(const double &pt, const double &eta);

    /*Loads the reweight hists from g_reweight_pairs and multiplies returns the weight associated with the proper
    bin in the histogram*/
    double getReweight();
    double getReweight_allSR(TString SR);
    /*This method stores fixes to the evt_scale1fb in the event of file corruptions.
    It's basically just a lookup table*/
    double scale1fbFix();

    /*Main function for determining the weights for each event*/
    double getWeight(TString SR = "");

    /*Returns the weight associated with the photon prescales*/
    double getPrescaleWeight();

    /*Computes Rsfof from the event-by-event description*/
    double computeRsfof(int unc_mode = 0); 
    
    int compute_isotrack_mt2();
    //=============================
    // Cuts
    //=============================

    /*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
    bool passSignalRegionCuts();

    //New SR and VR cuts
    bool passSRACuts();
    bool passSRAbCuts();
    bool passVRACuts();

    bool passSRBCuts();
    bool passSRBbCuts();
    bool passVRBCuts();

    bool passSRCCuts();
    bool passSRCbCuts();
    bool passVRCCuts();

    bool passSRVZCuts();
    bool passVRWZCuts();

    bool passSRVZBoostedCuts();
    bool passVRWZBoostedCuts();

    bool passSRHZCuts();
    bool passVRHZCuts();

    bool passStrongSRCuts();
    bool passEWKSRCuts();

    bool passStrongVRCuts();
    bool passEWKVRCuts();

    bool passInclusiveCuts();

    /*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
    over samples tagged as "rares". This is only neccesary for the full prediction.*/
    bool passRareCuts();

    /*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/
    bool passSUSYSignalCuts();

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

    bool InEtaPhiVetoRegion(float eta, float phi, int year);

    /*Loads the proper TH2 for the given SUSY sample which contains the BTag and ISR weights if running SUSY MC.*/
    void updateSUSYBtagISRNorms();

    /*Loads Pt reweighting histograms, pileup reweighting hists, and efficiency hists (which are no longer used really). Also sets up goodrun list*/
    void setupExternal(TString savePath);

    //Photon HLT Test Histograms
    //void PhotonHLTTest();
    //void FillTriggerHistograms();
    //


    //R_sfof parameter variables
    std::unordered_map<std::string,float> rMuEParameters; 

    //Histogram stuff

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
    int isotrack_mt2;
    bool recompute_isotrack_flag=true;

    //SUSY variables

    double ISR_norm,btag_norm,ISR_norm_up,btag_heavy_norm_up,btag_light_norm_up;
    double dphi_gm;

    std::string commonHistPrefix;

    void initSyncFile(TString savePath);
    void writeSyncFile();
    void fillallHistograms(std::string prefix = "");
    void fillMassWindowHistograms(std::string prefix = "");
    void fillFSMETHists(std::string prefix = "");
    void fillCommonHists(std::string prefix = "");
    void fillPhotonCRHists(std::string prefix = "");
    void fillGammaMuCRHists(std::string prefix = "");
    void fillDileptonCRHists(std::string prefix = "");
    void fillClosureHists(std::string prefix = "");
    void fillSignalRegionHists(std::string prefix = "");
    void fillEcalHists(std::string prefix = "");
    void fillTChiWZHists(std::string prefix = "");
    void fillTChiHZHists(std::string prefix = "");
    void fillBoostedHists(std::vector<size_t> fatjet_indices,std::string prefix = "");
    bool passHEM1516Veto();
    double fatJetScaleFactor(int mode = 0);
    double tau21WP();
    //SR Hists comin' soon...
    //

    //SUSY signal Histograms
    void fillChiHists(std::string prefix = "");
    void fillGluLSPHists(std::string prefix = "");

    LorentzVector computeMht();

    public:
    ZMETLooper(int year=-1);
    int getYear(); //Get the year out to create output directories inside non-member functions
    int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);
};

//Global Vars
//
