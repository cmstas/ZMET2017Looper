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





    TString g_sample_name;
    TFile* currentFile;
    double g_scale_factor; //Holds scale factors for sample.
    int g_year;


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
    int g_nBJetMedium;
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

    /*Finds the pair of B tagged jets (csv medium) with dijet mass closest to the mass of the higgs*/

    /*Builds MT2b from two highest CSV jets*/

    /*Builds Mbb from two highest CSV jets*/

    /*This function gets the MT2 built out of the two Bjets in an event, no guarentee
    is made about selecting the highest csv jets*/

    /*Builds MT2 for the two leading Bjets unless select_closest_higgs_mass is set, in which case it
    builds it out of the two bjets with dijet mass nearest the mass of the higgs.*/
    double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false);

    /*Returns boson Pt, determines whether sample is gjets or zjets first*/
    double bosonPt();

    /* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/

    /* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/

    /*Returns position in the gen particles for the ewk boson and first quark pair prodcued from the EWK Boson mother and SUSY grandmother.

  The function looks for a pair of quarks next to each other in the gen record with a mother that's the proper boson and a SUSY grandma.
  Then it checks that their diquark mass is within 10 GeV of boson's resonance mass, if this is true, the location of the pair is noted.

  Next it searches through the gen record backwards starting from the entry prior to the quarks position when they can be found, on cmd line it
  looked like ewk boson was normally the previous entry) and finds a EWK boson of proper flavor. If that boson has pt within 10 GeV of the diquark
  system, the boson's position is noted.

  If no boson can be found, -1 is returned as the first position.
  If no quarks can be found, -1 is returned as the second positon.*/

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

    //=============================
    // Event Weight Assignment
    //=============================

    /*Goes through the chain of weight_from config options down to a config which does not have weight_from and
    then adds a pair (config_name, hist_file) to the vector g_reweight_pairs.

    For now this is depricated: NEEDS TO BE UPDATED WITH NEW CODE FIXES*/
    //=============================
    // Cuts
    //=============================

    /*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
    bool passSignalRegionCuts();

    //New SR and VR cuts
     bool passInclusiveCuts();

    /*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
    over samples tagged as "rares". This is only neccesary for the full prediction.*/

    /*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/

    /*Front end method to "Dorky" duplicate removal*/
    bool isDuplicate();

    /*Checks for MET filters*/
    bool passMETFilters();

    /*Holds baseline cuts*/
    bool passBaseCut();

    bool passNJetsCut();
    bool passNBJetsCut();
    bool passMETCut();
    bool passIsotrackCut();

    /*Ensures events from the ee/mumu/emu dataset pass the trigger for that type of event and for ee and emu ensures they don't pass other triggers.*/
    bool passETHDileptonDataCleanse();

    /*Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples*/
    bool passEMuSelections();

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
    
    //File writing stuff
    fstream* syncFile_baseline;
    fstream* syncFile_emu;
    fstream* syncFile_njets;
    fstream* syncFile_metFilter;
    fstream* syncFile_met;
    fstream* syncFile_isotracks;

    //Histogram stuff


    double sumMETFilters;

    //misc stuff

    //SUSY variables

    void initSyncFiles(TString savePath);
    void writeSyncFile(fstream *syncFile);
    void closeSyncFiles();



    public:
    ZMETLooper(int year=-1);
    int getYear(); //Get the year out to create output directories inside non-member functions
    int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);
};

//Global Vars
//
