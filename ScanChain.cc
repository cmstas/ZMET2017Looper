#include "ScanChain.h"

static const int n_weight_log_bins = 54;
const double weight_log_bins[n_weight_log_bins+1] = {-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -.9, -.8, -.7, -.6, -.5, -.4, -.3, -.2, -.1, -.09, -.08, -.07, -.06, -.05, -.04, -.03, -.02, -.01, 0, .01, .02, .03, .04, .05, .06, .07, .08, .09, .1, .2, .3, .4, .5, .6, .7, .8, .9, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

static const int n_metbins_wide_std = 6;
const double metbins_wide_std[n_metbins_wide_std+1] = {0, 50, 100, 150, 225, 300, 500};

static const int n_ptbins_std = 20;
    const double ptbins_std[n_ptbins_std+1] = {0, 11,22,27,33,37,40,48,55, 70,85,95, 105,120, 135, 158,180,195, 210,500, 6000};

    static const int n_ptbins_fine = 51;
    const double ptbins_fine[n_ptbins_fine+1] = {0, 22, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 300, 350, 400, 6001};


    static const int n_gluino_bins_t5zznat = 30;
    const double gluino_bins_t5zznat[n_gluino_bins_t5zznat+1]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000,2150.000000,2200.000000,2250.000000,2300.000000,2350.000000,2400.000000};

    static const int n_lsp_bins_t5zznat = 79;
    const double lsp_bins_t5zznat[n_lsp_bins_t5zznat+1] ={25.000000,50.000000,100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,790.000000,800.000000,850.000000,890.000000,900.000000,950.000000,990.000000,1000.000000,1040.000000,1050.000000,1090.000000,1100.000000,1140.000000,1150.000000,1190.000000,1200.000000,1240.000000,1250.000000,1290.000000,1300.000000,1340.000000,1350.000000,1390.000000,1400.000000,1440.000000,1450.000000,1490.000000,1500.000000,1540.000000,1550.000000,1590.000000,1600.000000,1640.000000,1650.000000,1690.000000,1700.000000,1740.000000,1750.000000,1790.000000,1800.000000,1840.000000,1850.000000,1890.000000,1900.000000,1940.000000,1950.000000,1990.000000,2000.000000,2040.000000,2050.000000,2090.000000,2100.000000,2140.000000,2150.000000,2190.000000,2200.000000,2240.000000,2250.000000,2290.000000,2300.000000,2340.000000,2350.000000,2390.000000,2400.000000};

    static const int n_met_bins_t5zznat = 5;
//    const double met_bins_t5zznat[n_met_bins_t5zznat+1] = {50, 100, 150, 250, 350, 6000};
    const double met_bins_t5zznat[n_met_bins_t5zznat+1] = {50,100,150,230,300,6000};
    
    static const int n_met_bins_t5zznat_SRC = 4;
    const double met_bins_t5zznat_SRC[n_met_bins_t5zznat_SRC+1] = {50,100,150,250,6000};

     //MET bins from the old analysis

    static const int n_met_bins_t5zznat_2016 = 4;
    const double met_bins_t5zznat_2016[n_met_bins_t5zznat_2016+1] = {50,100,150,250,6000};
    static const int n_met_bins_t5zznat_2016_SRC = 3;
    const double met_bins_t5zznat_2016_SRC[n_met_bins_t5zznat_2016_SRC+1] = {50,100,150,6000};

    //==============================
    //T5ZZ contrived binning
    //==============================

    static const int n_gluino_bins_t5zzcontrived = 32;
    const double gluino_bins_t5zzcontrived[n_gluino_bins_t5zzcontrived+1]={800.000000,900.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000,2150.000000,2200.000000,2250.000000,2300.000000,2350.000000,2400.000000};

    static const int n_lsp_bins_t5zzcontrived = 54;
    const double lsp_bins_t5zzcontrived[n_lsp_bins_t5zzcontrived+1]={100.000000,150.000000,200.000000,250.000000,300.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000,750.000000,800.000000,850.000000,900.000000,950.000000,1000.000000,1050.000000,1100.000000,1150.000000,1200.000000,1250.000000,1300.000000,1350.000000,1400.000000,1450.000000,1500.000000,1550.000000,1600.000000,1650.000000,1700.000000,1750.000000,1800.000000,1850.000000,1900.000000,1950.000000,2000.000000,2050.000000,2100.000000,2150.000000,2200.000000,2250.000000,2300.000000,2350.000000,2400.000000};

    static const int n_met_bins_t5zzcontrived = 5;
    const double met_bins_t5zzcontrived[n_met_bins_t5zzcontrived+1] = {50, 100, 150, 250, 350, 6000};

    //==============================
    //TChiWZ model natural binning
    //==============================
    static const int n_gluino_bins_tchiwz = 38;
    const double gluino_bins_tchiwz[n_gluino_bins_tchiwz+1]={100.000000,125.000000,150.000000,175.000000,200.000000,225.000000,250.000000,275.000000,300.000000,325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000,725.000000,750.000000,775.000000,800.000000,825.000000,850.000000,875.000000,900.00000,925.000000,950.000000,975.000000,1000.000000,1025.000000,1050.000000};

    static const int n_lsp_bins_tchiwz = 142;
    const double lsp_bins_tchiwz[n_lsp_bins_tchiwz+1] = {1.000000,5.000000,10.000000,15.000000,20.000000,25.000000,30.000000,35.000000,40.000000,45.000000,50.000000,55.000000,60.000000,65.000000,70.000000,75.000000,80.000000,85.000000,90.000000,93.000000,95.000000,100.000000,105.000000,110.000000,115.000000,118.000000,120.000000,125.000000,130.000000,135.000000,140.000000,143.000000,145.000000,150.000000,155.000000,160.000000,165.000000,168.000000,170.000000,175.000000,180.000000,185.000000,190.000000,193.000000,195.000000,200.000000,205.000000,210.000000,215.000000,218.000000,220.000000,225.000000,230.000000,235.000000,240.000000,243.000000,245.000000,250.000000,255.000000,260.000000,265.000000,268.000000,270.000000,275.000000,280.000000,285.000000,290.000000,293.000000,295.000000,300.000000,305.000000,310.000000,315.000000,318.000000,320.000000,325.000000,330.000000,335.000000,340.000000,343.000000,345.000000,350.000000,355.000000,360.000000,365.000000,368.000000,370.000000,375.000000,380.000000,385.000000,390.000000,393.000000,395.000000,400.000000,405.000000,410.000000,415.000000,418.000000,420.000000,425.000000,430.000000,435.000000,440.000000,443.000000,445.000000,450.000000,455.000000,460.000000,465.000000,468.000000,470.000000,475.000000,480.000000,485.000000,490.000000,493.000000,495.000000,500.000000,505.000000,510.000000,515.000000,518.000000,520.000000,525.000000,530.000000,535.000000,540.000000,543.000000,545.000000,550.000000,555.000000,560.000000,565.000000,568.000000,570.000000,575.000000,580.000000,585.000000,590.000000,593.000000,595.000000,600.000000,610.000000};

    static const int n_met_bins_tchiwz_resolved = 5;
    const double met_bins_tchiwz_resolved[n_met_bins_tchiwz_resolved+1] = {50, 100, 150, 250, 350, 6000};

    static const int n_met_bins_tchiwz_SRHZ = 4;
    const double met_bins_tchiwz_SRHZ[n_met_bins_tchiwz_SRHZ+1] = {50,100,150,250,6001};
    
    static const int n_met_bins_tchiwz_boosted = 6;
    const double met_bins_tchiwz_boosted[n_met_bins_tchiwz_boosted+1] = {50,100,200,300,400,500,6001};


    //==============================
    //TChiWZ EXT model natural binning
    //==============================
    static const int n_gluino_bins_tchiwz_ext = 22;
    const double gluino_bins_tchiwz_ext[n_gluino_bins_tchiwz_ext+1]={325.000000,350.000000,375.000000,400.000000,425.000000,450.000000,475.000000,500.000000,525.000000,550.000000,575.000000,600.000000,625.000000,650.000000,675.000000,700.000000,750.000000,800.000000,850.000000,900.000000,950.000000,1000.000000,1050.000000};

    static const int n_lsp_bins_tchiwz_ext = 20 ;
    const double lsp_bins_tchiwz_ext[n_lsp_bins_tchiwz_ext+1]={300.000000,305.000000,310.000000,315.000000,318.000000,320.000000,325.000000,330.000000,335.000000,340.000000,343.000000,345.000000,350.000000,400.000000,450.000000,500.000000,550.000000,600.000000,650.000000,700.000000};

    static const int n_met_bins_tchiwz_ext = 5;
    const double met_bins_tchiwz_ext[n_met_bins_tchiwz_ext+1] = {50, 100, 150, 250, 350, 6000};

    //==============================
    //TChiHZ model natural binning
    //==============================
    //!!!!!!!!!!!!!!!!!!
    // Be sure to change the lowest bin to 125 to match the tchizz model.
    //!!!!!!!!!!!!!!!!!!
    static const int n_chi_bins_tchihz = 51;
    const double chi_bins_tchihz[n_chi_bins_tchihz+1] = {125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000,1025,1050,1075,1100,1125,1150,1175,1200,1225,1250,1275,1300,1325,1350,1375,1400};

    static const int n_met_bins_tchihz = 5;
    const double met_bins_tchihz[n_met_bins_tchihz+1] = {50, 100, 150, 250, 350, 6000};


    //==============================
    //TChiZZ model natural binning
    //==============================
    static const int n_chi_bins_tchizz = 52;
    const double chi_bins_tchizz[n_chi_bins_tchizz+1] = {100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000,1025,1050,1075,1100,1125,1150,1175,1200,1225,1250,1275,1300,1325,1350,1375,1400};

    static const int n_met_bins_tchizz_resolved = 5;
    const double met_bins_tchizz_resolved[n_met_bins_tchizz_resolved+1] = {50, 100, 150, 250, 350, 6000};

    static const int n_met_bins_tchizz_SRHZ = 4;
    const double met_bins_tchizz_SRHZ[n_met_bins_tchiwz_SRHZ+1] = {50,100,150,250,6001};
    
    static const int n_met_bins_tchizz_boosted = 6;
    const double met_bins_tchizz_boosted[n_met_bins_tchiwz_boosted+1] = {50,100,200,300,400,500,6001};


    static const int n_SR = 15;
    const TString SRs[n_SR] = {"SRA","SRB","SRC","SRAb","SRBb","SRCb","SRVZResolved","SRVZBoosted","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"};


ZMETLooper::ZMETLooper(int year)
{
    nDuplicates = 0;
    year_fromCommandLine = year;
    num_events_veto_ttbar = 0;
    num_events_veto_ttgamma = 0;
    MCTriggerEmulation = true;

    rootdir = gDirectory->GetDirectory("Rint:");
    currentFile = nullptr;
    g_scale_factor = 1;
    printStats = false;
    printFail = false;

    dil_flavor = -1;
}

int ZMETLooper::getYear()
{
    return year_fromCommandLine;
}

LorentzVector ZMETLooper::computeMht()
{
    LorentzVector sumMht_p4 = LorentzVector(0,0,0,0);
    for(auto &it:phys.jets_p4())
    {
        sumMht_p4 -= it;
    }
    if(conf->get("event_type") != "photon")
    {
        for(auto &it:phys.lep_p4())
        {
            sumMht_p4 -= it;
        }
    }
    else
    {
        for(auto &it:phys.gamma_p4())
        {
            sumMht_p4 -= it;
        }
    }
    return sumMht_p4;
}


pair<int, int> ZMETLooper::getMostBlike(){
  /* returns two most B-like jet indicies */
  int first_index = 0;
  int second_index = 1;

  if (g_jets_csv.at(first_index) < g_jets_csv.at(second_index)){
    first_index = 1;
    second_index = 0;
  }

  for (int i = 2; i < (int) g_jets_p4.size(); i++){
    if (g_jets_csv.at(first_index) < g_jets_csv.at(i)){
      second_index = first_index;
      first_index = i;
    }
    else if (g_jets_csv.at(second_index) < g_jets_csv.at(i)){
      second_index = i;
    }
  }

  return make_pair(first_index, second_index);
}

pair<int,int> ZMETLooper::getClosestBPairToHiggsMass(){
  int first = 0;
  int second = 1;

  if (g_jets_p4.size()<2){
    cout<<"Going to throw error finding closest B pair: Less than two jets!"<<endl;
    std::stringstream message;
    message<<"Can not find closest B pair to Higgs for event: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" the event only has "<<g_jets_p4.size()<<" jets";
    throw std::underflow_error(message.str());
  }

  double dist = abs((g_jets_p4.at(0)+g_jets_p4.at(1)).M() - 125.0);

  for (int i = 0; i < (int) g_jets_p4.size(); i++) {
    for (int j = i+1; j < (int) g_jets_p4.size(); j++) {
      if (abs((g_jets_p4.at(i)+g_jets_p4.at(j)).M() - 125 ) < dist){
        first = i;
        second = j;
      }
    }
  }

  return make_pair(first,second);
}

double ZMETLooper::getMT2B(){
  /*Builds MT2b from two highest CSV jets*/

  pair<int, int> b_index = getMostBlike();

  double mt2_1=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first)+phys.lep_p4().at(0), g_jets_p4.at(b_index.second)+phys.lep_p4().at(1), 0, 0);
  double mt2_2=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first)+phys.lep_p4().at(1), g_jets_p4.at(b_index.second)+phys.lep_p4().at(0), 0, 0);

  if (mt2_1 > mt2_2){
    return mt2_2;
  }
  else{
    return mt2_1;
  }
}

double ZMETLooper::getMbb(){
  /*Builds Mbb from two highest CSV jets*/

  pair<int, int> b_index = getMostBlike();

  return (g_jets_p4.at(b_index.first)+g_jets_p4.at(b_index.second)).M();
}

double ZMETLooper::getMT2ForBjets(bool select_highest_csv/*=false*/){
  /*This function gets the MT2 built out of the two Bjets in an event, no guarentee is made about selecting the highest csv jets*/
  double mt2;
  if (select_highest_csv){
    pair<int, int> b_index = getMostBlike();
    //make sure first index points to the higher csv of the first two jets
    mt2=MT2(g_met, g_met_phi, g_jets_p4.at(b_index.first), g_jets_p4.at(b_index.second), 0, 0);
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    //cout<<__LINE__<<endl;
    mt2=MT2(g_met, g_met_phi, g_jets_medb_p4.at(0), g_jets_medb_p4.at(1), 0, 0);
  }
  //cout<<__LINE__<<endl;
  return mt2;
}

double ZMETLooper::getMT2HiggsZ(bool select_highest_closest_higgs_mass/*=false*/){

  double mt2;

  if(select_highest_closest_higgs_mass){
    pair<int, int> jet_indexes = getClosestBPairToHiggsMass();
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(g_met, g_met_phi, g_jets_p4.at(jet_indexes.first)+g_jets_p4.at(jet_indexes.second), phys.lep_p4().at(0)+phys.lep_p4().at(1), 0, 0);
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(g_met, g_met_phi, g_jets_medb_p4.at(0)+g_jets_medb_p4.at(1), phys.lep_p4().at(0)+phys.lep_p4().at(1), 0, 0);
  }

  return mt2;
}

double ZMETLooper::bosonPt(){
  // Returns boson Pt, determines whether sample is gjets or zjets first.
  if (conf->get("event_type") == "dilepton") {
    return phys.dilpt();
  }
  else{
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      return phys.gamma_pt().at(0);
    }
    else
      return 0;
  }
}

double ZMETLooper::getMTLepMET(short id/*=0*/){
  /* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
  return sqrt(2*g_met*phys.lep_p4().at(id).pt()*(1 - cos(g_met_phi - phys.lep_p4().at(id).phi())));

}

double ZMETLooper::getdRGammaLep(short id/*=0*/){
  /* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
  double dPhi=acos(cos(phys.gamma_p4().at(0).phi() - phys.lep_p4().at(id).phi()));
  double dEta=phys.gamma_p4().at(0).eta() - phys.lep_p4().at(id).eta();

  return sqrt(pow(dPhi, 2) + pow(dEta, 2));
}

pair<int,int> ZMETLooper::getSUSYHadDecayBoson(){
  /*Returns position in the gen particles for the ewk boson and first quark pair prodcued from the EWK Boson mother and SUSY grandmother.

  The function looks for a pair of quarks next to each other in the gen record with a mother that's the proper boson and a SUSY grandma.
  Then it checks that their diquark mass is within 10 GeV of boson's resonance mass, if this is true, the location of the pair is noted.

  Next it searches through the gen record backwards starting from the entry prior to the quarks position when they can be found, on cmd line it
  looked like ewk boson was normally the previous entry) and finds a EWK boson of proper flavor. If that boson has pt within 1 GeV of the diquark
  system, the boson's position is noted.

  If no boson can be found, -1 is returned as the first position.
  If no quarks can be found, -1 is returned as the second positon.
  */

  int quarks_pos = -1; //pair produced are always consecutive rows, so only need one
  int boson_pos = -1;

  int target_pdgId, target_mass;
  double target_pt = -1;
  int SUSY_low_id = 1000000;
  int quarks_high_id = 6;
  int boson_search_begin = (int) phys.genPart_pdgId().size() - 1;

  if (TString(currentFile->GetTitle()).Contains("tchiwz_80x")) {
    target_pdgId = W_PDG_ID;
    target_mass = W_MASS;
  }
  else if (TString(currentFile->GetTitle()).Contains("tchizz_80x")) {
    target_pdgId = Z_PDG_ID;
    target_mass = Z_MASS;
  }
  else if (TString(currentFile->GetTitle()).Contains("tchihz_80x")) {
    target_pdgId = H_PDG_ID;
    target_mass = H_MASS;
  }
  else{
    std::stringstream message;
    message<<"Can not get hadron from SUSY decay, sample name is not tchihz, tchiwz, or tchizz, got: "<<currentFile->GetTitle();
    throw std::invalid_argument(message.str());
  }


  // loop through and find quark pairs
  for (int i = 0; i<(int)phys.genPart_pdgId().size() - 1; i++){
    if ( (abs(phys.genPart_pdgId().at(i)) <= quarks_high_id ) && (abs(phys.genPart_pdgId().at(i+1)) <= quarks_high_id) ) { //quarks have id 0 through 6
      if ((phys.genPart_motherId().at(i) != target_pdgId) || (phys.genPart_motherId().at(i+1) != target_pdgId)) continue;
      //cout<<__LINE__<<endl;
      if ((phys.genPart_grandmaId().at(i) < SUSY_low_id) || (phys.genPart_grandmaId().at(i+1) < SUSY_low_id)) continue;
      //cout<<__LINE__<<endl;
      if (abs((phys.genPart_p4().at(i) + phys.genPart_p4().at(i+1)).M() - target_mass) < 10){
        quarks_pos = i;
        boson_search_begin = quarks_pos - 1;
        target_pt = (phys.genPart_p4().at(i) + phys.genPart_p4().at(i+1)).pt();
        break;
      }
    }
  }


  //loop through other particles looking for EWK bosons.
  for (int i = boson_search_begin; i>=0; i--) {
    //cout<<__LINE__<<endl;
    if ( abs(phys.genPart_pdgId().at(i)) == target_pdgId ) { //find proper boson
      //cout<<__LINE__<<endl;
      if (abs(phys.genPart_p4().at(i).pt() - target_pt) < 1){ //check it has pt consistent with chosen quarks.
        //cout<<__LINE__<<endl;
        boson_pos = i;
      }
      else if ((target_pt == -1) && (abs(phys.genPart_motherId().at(i)) > SUSY_low_id)) { //If the quarks weren't found, just take the first boson that's proper with the correct mother ID.
        //cout<<__LINE__<<endl;
        boson_pos = i;
      }
    }
  }


  return make_pair(boson_pos, quarks_pos);
}

double ZMETLooper::DeltaR(const LorentzVector p1, const LorentzVector p2){
  /*Returns the DeltaR between objects p1 and p2.*/

  return sqrt( (p1.eta() - p2.eta())*(p1.eta() - p2.eta())+(p1.phi() - p2.phi())*(p1.phi() - p2.phi()) );
}

TFile VetoFile("External/veto_etaphi.root");
TH2F* veto_etaphi_16 = (TH2F*) VetoFile.Get("etaphi_veto_16");
TH2F* veto_etaphi_17 = (TH2F*) VetoFile.Get("etaphi_veto_17");
TH2F* veto_etaphi_18 = (TH2F*) VetoFile.Get("etaphi_veto_18");

bool ZMETLooper::InEtaPhiVetoRegion(float eta, float phi, int year) {
  if (fabs(eta) > 2.4) return false;
  TH2F* veto_hist = veto_etaphi_16;
  if (year == 2017) veto_hist = veto_etaphi_17;
  if (year == 2018) veto_hist = veto_etaphi_18;
  int flag =  (int) veto_hist->GetBinContent(veto_hist->FindBin(eta,phi));
  if(flag == 1)
      return true;
  else
      return false;
}
//=============================
// Triggers
//=============================

bool ZMETLooper::passPhotonEmulatedTrigger() {
  if( phys.gamma_r9()            .at(0) < 0.92 ) return false;
  if( phys.gamma_hOverE()        .at(0) > 0.2  ) return false;
  if( phys.gamma_hollowtkiso03() .at(0) > 5    ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0053  ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.014   ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0034  ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.0139  ) return false;

  return true;
}

bool ZMETLooper::passPhotonTriggers(){

  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
      //year based shit goes here
            //165 is the highest trigger in 2016, while in 2017 and 2018 triggers go all the way up to 200
            if(g_year != 2016 && phys.HLT_Photon200() > 0 && phys.gamma_pt().at(0) > 210) return true; //need to do trigger efficiency tests
            if (!phys.HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 180)
            {
                if(g_year == 2016) return true;
                else if(phys.gamma_pt().at(0) < 210) return true; 
            }
            else if(phys.HLT_Photon165_HE10() > 0 && phys.gamma_pt().at(0) > 180 && g_year == 2016)
            {
                return true;
            }
            else if( !phys.HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 && phys.gamma_pt().at(0) < 180) return true;
            else if( !phys.HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 && phys.gamma_pt().at(0) < 135  ) return true;
            else if( !phys.HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 && phys.gamma_pt().at(0) < 105   ) return true;
            else if( !phys.HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 && phys.gamma_pt().at(0) < 85    ) return true;
            return false;
    }
}

bool ZMETLooper::passMuonTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    //cout<<__LINE__<<endl;
          bool triggers = false;
          if(g_year == 2017)
          {

            triggers = phys.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8() || phys.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ() || phys.HLT_Mu37_TkMu27();
          }

          else if(g_year == 2016)
          {
            triggers = phys.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() || /*phys.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL() ||*/ phys.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ() || phys.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ() || phys.HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ() || phys.HLT_Mu27_TkMu8() || phys.HLT_Mu30_TkMu11();
          }
          
          else if(g_year == 2018)
          {
              triggers = phys.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8() || phys.HLT_Mu37_TkMu27();
          }
        return triggers;
  }
}

bool ZMETLooper::passElectronTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    //cout<<__LINE__<<endl;
    bool eleTriggers = false;
    if(g_year == 2017)
    {
        eleTriggers = phys.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL() || phys.HLT_DoubleEle33_CaloIdL_MW() || phys.HLT_DoubleEle25_CaloIdL_MW() || phys.HLT_DoubleEle27_CaloIdL_MW_Edge();
    }
    else if(g_year == 2016)
    {
        eleTriggers = phys.HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_DoubleEle33_CaloIdL_GsfTrkIdVL() || phys.HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW();
    }

    else if(g_year == 2018)
    {
        eleTriggers = phys.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL() || phys.HLT_DoubleEle25_CaloIdL_MW();
    }
    return eleTriggers;
  }
}

bool ZMETLooper::passEMuTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }

  else{
      bool trigger = false;
      if(g_year == 2017)
      {
      trigger = phys.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu27_Ele37_CaloIdL_MW() || phys.HLT_Mu37_Ele27_CaloIdL_MW();
      }

      else if(g_year == 2016)
      {
          trigger = phys.HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL() /*|| phys.HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ()*/ || phys.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() || phys.HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL() || phys.HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL();
      }

      else if(g_year == 2018)
      {
          trigger = phys.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ() || phys.HLT_Mu27_Ele37_CaloIdL_MW() || phys.HLT_Mu37_Ele27_CaloIdL_MW();
      }
      return trigger;
  }
}

bool ZMETLooper::passSingleMuTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    return (phys.HLT_singleMu());
  }
}

bool ZMETLooper::passLeptonHLTs(){
    if (dil_flavor == 2)
        return passEMuTriggers();
    else if (dil_flavor == 1)
      return passMuonTriggers();
    else if ( dil_flavor == 0 )
      return passElectronTriggers();
    else
      return false;
}

//=============================
// Has Good Event Functions
//=============================

int ZMETLooper::hasGoodZ(){

  if( phys.nlep() < 2         ){
    numEvents->Fill(10);
    if (printFail) cout<<phys.evt()<<" :Failed 2 lepton Z cut"<<endl;
    return -1; // require at least 2 good leptons
  }

  dil_flavor = phys.hyp_type();
  if(conf->get("dil_flavor") == "all")
  {
    if(!(dil_flavor == 1 || dil_flavor == 0 || dil_flavor == 2))
    {
        numEvents->Fill(15);
        if(printFail) cout<<"Require 2 leptons, but event does not have it"<<endl;
        return -1;
    }
  }

  else if (conf->get("dil_flavor") == "emu"){ //only true for ttbar estimate
    if (! (phys.hyp_type() == 2) ){ //require explicit emu event
      numEvents->Fill(15);
      if (printFail) cout<<phys.evt()<<" :Failed not explicit e/mu Z cut, for ttbar only"<<endl;
      return -1; // require explicit opposite flavor event
    }
  }
  else if(conf->get("dil_flavor") == "ee")
  {
      if(!(phys.hyp_type() == 0))
    {
        numEvents->Fill(15);
        if(printFail) cout<<"Failed 2 electron requirement"<<endl;
        return -1;
    }
  }
  else if(conf->get("dil_flavor") == "mumu")
  {
    if(!(phys.hyp_type() == 1))
    {
        numEvents->Fill(15);
        if(printFail) cout<<"Failed 2 muon requirement"<<endl;
        return -1;
    }
  }
  else //ee+mumu sector
  {
    if( !( phys.hyp_type() == 0 || phys.hyp_type() == 1 ) ) {
        numEvents->Fill(15);
        if (printFail) cout<<phys.evt()<<" :Failed explicit mu/mu or e/e Z cut"<<endl;
        return -1; // require explicit same flavor event
        }
  }
    //if (printStats) { cout<<"hyp_type: "<<phys.hyp_type()<<" "; }

  if( phys.lep_pt().at(0) < 25)  {
    numEvents->Fill(11);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return -1; // leading lep pT > 25 GeV
  }


  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(1) < 20        ) {
    numEvents->Fill(12);
    if (printFail) cout<<phys.evt()<<" :Failed lep2 pt < 20 Z cut"<<endl;
    return -1; // tailing lep pT > 20 GeV
  }


  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return -1; // eta < 2.4
  }


  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) {
    numEvents->Fill(14);
    if (printFail) cout<<phys.evt()<<" :Failed lep2 eta > 2.4 Z cut"<<endl;
    return -1; 
  }
  
  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return -1;
  }

  //cout<<__LINE__<<endl;

    z_pt = (conf->get("z_pt") != "") ? stoi(conf->get("z_pt")) : 55;
    if( phys.dilpt() < z_pt ){
      numEvents->Fill(26);
      if (printFail) cout<<phys.evt()<<" :Failed Z pt cut"<<endl;
      return -1;
    }
    //cout<<__LINE__<<endl;

    if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 ) {
      numEvents->Fill(18);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 in xition region Z cut"<<endl;
      return -1; // veto xition region
    }

    //cout<<__LINE__<<endl;

    if( phys.dRll() < 0.1)   {
      numEvents->Fill(19);
      if (printFail) cout<<phys.evt()<<" :Failed deltaR Z cut"<<endl;
      return -1;
    }

    //cout<<__LINE__<<endl;

  if (conf->get("fastsim") != "true" && !passLeptonHLTs()){
    numEvents->Fill(20);
    if (printFail) cout<<phys.evt()<<" :Failed HLT Z cut"<<endl;
    return -1;
  }

  //cout<<__LINE__<<endl;

  if( !(phys.evt_type() == 0 ) ) {
    numEvents->Fill(21);
    if (printFail) cout<<phys.evt()<<" :Failed evt_type=0 Z cut"<<endl;
    return -1; // require opposite sign
  }
  //if (printStats) { cout<<"evt_type: "<<phys.evt_type()<<" "; }

  //cout<<__LINE__<<endl;
  //Set up Dilepton Masses
  double dilmass_low, dilmass_high;

  if (conf->get("dilmass_low") == "") {
    dilmass_low = 86;
  }
  else{
    dilmass_low = stod(conf->get("dilmass_low"));
  }

  if (conf->get("dilmass_high") == ""){
    dilmass_high = 96;
  }
  else{
    dilmass_high = stod(conf->get("dilmass_high"));
  }

  //If all SRs are done together, we want emu
  //to use the wider dilmass window
  if(conf->get("signal_region") == "all")
  {
    if(dil_flavor == 2)
    {
        dilmass_low = 20;
        dilmass_high = -1;
    }
  }

  if(conf->get("FS_mass_window_study") == "true")
  {
      if(dil_flavor == 2)
      {
        dilmass_low = 20;
        dilmass_high = -1;
      
      }
      else
         return -1;
  }

  if( phys.dilmass() < dilmass_low) {
    numEvents->Fill(22);
    if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return -1; // on-Z
  }
  if( phys.dilmass() > dilmass_high && dilmass_high != -1) {
    numEvents->Fill(22);
    if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return -1; // on-Z
  }

  return dil_flavor;
}

bool ZMETLooper::hasGoodPhoton(){
  if( phys.ngamma() <  1 ) {
    numEvents->Fill(24);
    if (printFail) cout<<phys.evt()<<" :Failed at least 1 photon cut"<<endl;
    return false; // require at least 1 good photon
  }

  if( phys.evt_type() != 2 ) {
    numEvents->Fill(25);
    if (printFail) cout<<phys.evt()<<" :Failed explicitly evt_type2 photon cut"<<endl;
    return false; // photon + jets events
  }

  if( phys.gamma_pt().at(0) < 55 ) {
    numEvents->Fill(26);
    if (printFail) cout<<phys.evt()<<" :Failed pt < 55 photon cut"<<endl;
    return false; // photon pt > 55 GeV
  }

  if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
    numEvents->Fill(27);
    if (printFail) cout<<phys.evt()<<" :Failed gamma in xition region photon cut"<<endl;
    return false; // veto xition region
  }

  if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
    numEvents->Fill(28);
    if (printFail) cout<<phys.evt()<<" :Failed gamma eta > 2.4 photon cut"<<endl;
    return false; // photon in EC or EB
  }

  if( phys.gamma_hOverE().at(0) > 0.1 ) {
    numEvents->Fill(29);
    if (printFail) cout<<phys.evt()<<" :Failed gamma hOverE photon cut"<<endl;
    return false; // H/E < 0.1
  }


  if( phys.matched_emf() < 0.7 ) {
    numEvents->Fill(30);
    if (printFail) cout<<phys.evt()<<" :Failed matched_emf photon cut"<<endl;
    return false; // jet neutral EM fraction cut
  }

  if (conf->get("ECalTest") == ""){
    if( acos( cos( phys.gamma_phi().at(0) - g_met_phi ) ) < 0.4 ) {
      numEvents->Fill(31);
      if (printFail) cout<<phys.evt()<<" :Failed photons aligned with MET photon cut"<<endl;
      return false; // kill photons aligned with MET
    }
  }

  if(conf->get("photon_ecal_veto") == "true" && InEtaPhiVetoRegion(phys.gamma_eta().at(0),phys.gamma_phi().at(0),g_year))
  {
      numEvents->Fill(76);
      if(printFail) cout<<"Photon in veto region"<<endl;
      return false;
  }

  if( phys.elveto() ) {
    numEvents->Fill(32);
    if (printFail) cout<<phys.evt()<<" :Failed electron pixel veto photon cut"<<endl;
    return false; // veto pixel match
  }

  if(conf->get("event_type") == "photon"){

    if (phys.isData() && (! passPhotonTriggers()) ){
      numEvents->Fill(52);
      if (printFail) cout<<phys.evt()<<" :Failed Photon trigger cut"<<endl;
      return false;
    }
  }

  dphi_gm = acos(cos(g_met_phi - phys.gamma_p4().at(0).phi()));
  if(conf->get("dPhi_Gamma_MET_max") != "")
  {
    if(dphi_gm > stod(conf->get("dPhi_Gamma_MET_max")))
    {
        numEvents->Fill(77);
        if(printFail) cout<<phys.evt()<<" :Failed dphi(gamma,MET) cut"<<endl;
        return false;
    }
  }

  if(conf->get("dPhi_Gamma_MET_min") != "")
  {
    if(dphi_gm < stod(conf->get("dPhi_Gamma_MET_min")))
    {
        numEvents->Fill(77);
        if(printFail) cout<<phys.evt()<<" :Failed dphi(gamma,MET) cut"<<endl;
        return false;
    }
  }

  if (conf->get("nisoTrack_5gev_max") != ""){
    if( phys.nisoTrack_5gev() > stoi(conf->get("nisoTrack_5gev_max")) ){
      numEvents->Fill(62);
      if (printFail) cout<<phys.evt()<<" :Failed nisotrack_5gev cut"<<endl;
      return false;
    }
  }

  if(conf->get("dr_min_photon_parton") != "")
  {
      if(*std::min_element(phys.gamma_drMinParton().begin(),phys.gamma_drMinParton().end()) < stoi(conf->get("dr_min_photon_parton")) || *std::min_element(phys.gamma_drMinParton().begin(),phys.gamma_drMinParton().end()) == 999)
          return false;
  }

  return true;
}

bool ZMETLooper::hasGoodGammaMu(){
  /*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
  It just checks muon quality stuff and then calls hasGoodPhoton()*/

  if( phys.nlep() < 1         ){
    numEvents->Fill(10);
    if (printFail) cout<<phys.evt()<<" :Failed 1 lepton GammaMu cut"<<endl;
    return false; // require at least 1 good leptons
  }
  //if (printStats) { cout<<"Number of Leptons: "<<phys.nlep()<<" "; }

  //cout<<__LINE__<<endl;

  if (abs(phys.lep_pdgId().at(0)) != 13){
    numEvents->Fill(61);
    if (printFail) cout<<phys.evt()<<" :Failed GammaMu muon check"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(0) < 25        ) {
    numEvents->Fill(11);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return false; // leading lep pT > 25 GeV
  }
  //if (printStats) { cout<<"lep1 pt: "<<phys.lep_pt().at(0)<<" "; }

  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }
  //if (printStats) { cout<<"lep1 eta: "<<phys.lep_p4().at(0).eta()<<" "; }

  //cout<<__LINE__<<endl;

  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if (conf->get("trigger_type") == "singleMu"){
    if(/*phys.isData() &&*/ (! passSingleMuTriggers()) ){
      numEvents->Fill(65);
      if (printFail) cout<<phys.evt()<<" :Failed Single Muon trigger cut"<<endl;
      return false;
    }
  }
  else if (conf->get("trigger_type") == "singleGamma"){
    if (/*phys.isData() &&*/ (! passPhotonTriggers()) ){
      numEvents->Fill(52);
      if (printFail) cout<<phys.evt()<<" :Failed Photon trigger cut"<<endl;
      return false;
    }
  }
  else{
    cout<<"No Trigger Type Set"<<endl;
    std::stringstream message;
    message<<"Invalid or missing trigger type set in config. Please check config variable \"trigger_type\".";
    throw std::invalid_argument(message.str());
    return false;
  }

  //cout<<__LINE__<<endl;

  return hasGoodPhoton();
}

bool ZMETLooper::hasGoodEvent() {
  if ( conf->get("event_type") == "photon"){
    return hasGoodPhoton();
  }
  else if (conf->get("event_type") == "photon_muon" ){
    return (hasGoodGammaMu());
  }
  else {
      dil_flavor = hasGoodZ();
      if(dil_flavor>=0)
        return true;
      else return false;
  }
}

//=============================
// Event Weight Assignment
//=============================

void ZMETLooper::readyReweightHists(){
  TString conf_name = conf->get("Name");

  //cout<<"FINDFIND Adding "<<conf->get("Name");

  cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
  TString rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
  TFile *reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
  g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
  g_reweight_pairs.back().first->SetDirectory(rootdir);
  reweight_file->Close();

  while (conf->get("weight_from") != "" ){
    conf->loadConfig(conf->get("weight_from"));
    //cout<<"FINDFIND Adding "<<conf->get("Name");
    cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
    rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
    reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
    g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
    g_reweight_pairs.back().first->SetDirectory(rootdir);
    reweight_file->Close();
  }

  conf->loadConfig(conf_name.Data());
  cout<<"Reweight hists loaded, proceeding with conf "<<conf->get("Name")<<endl;
}

void ZMETLooper::readyVPTReweight_allSR(TString save_path)
{
  TString vpt_reweight_path = save_path + conf->get("Name") + "_vpt_rwt.root";
  TString rwt_hist_name_suffix = "vpt_ratio";

  cout<<"Reweighting with "<<vpt_reweight_path<<endl;

  TFile *reweight_file = TFile::Open(vpt_reweight_path,"READ");
  for(auto &it:SRs)
  {
      if(conf->get("2016_reproduce") == "true" and it.Contains("Boosted"))
           continue;

      TString rwt_hist_name = it + rwt_hist_name_suffix;
      g_vpt_reweight_pairs[std::string(it.Data())] = (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(it+TString("_vpt_reweight_hist"));
      g_vpt_reweight_pairs[std::string(it.Data())]->SetDirectory(rootdir);
  }
  reweight_file->Close();
}


void ZMETLooper::readyVPTReweight(TString save_path){
  /* Adds the vpt reweighting histograms to the g_reweight_pairs vector */

  TString vpt_weight_path = save_path+conf->get("Name")+"_vpt_rwt.root";
  TString rwt_hist_name = "vpt_ratio";

  cout<<"Reweighting with "<<vpt_weight_path<<endl;

  TFile *reweight_file = TFile::Open(vpt_weight_path, "READ");

  //Add pair (vpt_weight, "vpt") to g_reweight_pairs
  g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("vpt_reweight_hist")),"vpt"));
  g_reweight_pairs.back().first->SetDirectory(rootdir);

  reweight_file->Close();
}

double ZMETLooper::getEff(const double &pt, const double &eta){
  /* Returns the trigger efficiency from g_pt_eff */
  if (abs(eta) < 1.4){
    return g_pt_eff_barrel->GetEfficiency(g_pt_eff_barrel->FindFixBin(pt));
  }
  else{
    return g_pt_eff_endcap->GetEfficiency(g_pt_eff_endcap->FindFixBin(pt));
  }
}

double ZMETLooper::getReweight(){
  double weight = 1;

  TH1D* rwt_hist;
  TString rwt_var;
  //cout<<"Size: "<<g_reweight_pairs.size()<<endl;
  for (int i=0; i<(int)g_reweight_pairs.size(); i++){
    rwt_hist = g_reweight_pairs.at(i).first;
    rwt_var = g_reweight_pairs.at(i).second;
    //cout<<rwt_var<<endl;

    if (rwt_var == "vpt"){
      //cout<<"Applying vpt reweight -- pt: "<<bosonPt()<<" weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()));
    }
    else if (rwt_var == "ht_wide"){
      //cout<<"Adding HT weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(ht))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(g_ht));
    }
    else if (rwt_var == "muonPT_endcap"){
      //cout<<"Adding muon pt trigger efficiency weight in endcap: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)))<<endl;
      if (phys.lep_p4().at(0).eta() > 1.6){
        weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)));
      }
    }
    else if (rwt_var == "muonPT_barrel"){
      //cout<<"Adding muon pt trigger efficiency weight in barrel: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)))<<endl;
      if (phys.lep_p4().at(0).eta() < 1.4){
        weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(phys.lep_pt().at(0)));
      }
    }
    else{
      std::stringstream message;
      message<<"Reweight varible is not a valid option, please choose vpt, or ht_wide, got: "<<rwt_var<<".";
      throw std::invalid_argument(message.str());
    }
  }

  return weight;
}

double ZMETLooper::getReweight_allSR(TString SR)
{
    /*I'm doing this under the assumption that in case of all SRs, the vpt reweights are found only in the g_vpt_reweight_pairs vector, and not in the g_reweight_pairs vector*/
    return g_vpt_reweight_pairs[std::string(SR.Data())]->GetBinContent(g_vpt_reweight_pairs[std::string(SR.Data())]->FindBin(bosonPt()));
}

double ZMETLooper::scale1fbFix(){
  /*This method stores fixes to the evt_scale1fb in the event of file corruptions. It's basically just a lookup table*/

  if (TString(currentFile->GetTitle()).Contains("sttw_antitop_nofullhaddecay_powheg")){
    //cout<<"Scale 1fb fixed for "<<TString(currentFile->GetTitle())<<endl;
    return 1.03;
  }
  else{
    //cout<<"Scale 1fb is good for "<<TString(currentFile->GetTitle())<<endl;
    return 1;
  }
}

double ZMETLooper::getWeight(TString SR){
  /*Gets the proper weight for the sample. */
  double weight=1;
  double ISR_norm, btag_norm;
  //cout<<__LINE__<<endl;
  if ( ! ( phys.isData() ) ){
    weight *= phys.evt_scale1fb();

    //Weight to some other lumi
    if ( conf->get("scaleTofb") != "" ){
      weight *= stod(conf->get("scaleTofb"));
    }
    else if(conf->get("auto_scale") == "true")
    {
      //scale up MCs automatically
      if(g_year == 2016)
        weight *= 35.9;
      else if(g_year == 2017)
        weight *= 41.5;
      else if(g_year == 2018)
        weight *= 59.76;
    }

    if (conf->get("pileup_reweight") == "true" and !phys.isData()){
      weight*=g_pileup_hist->GetBinContent(g_pileup_hist->FindBin(phys.nTrueInt()));
    }

    if (TString(conf->get("Name")).Contains("EWKSub")){
      weight *= -1; //EWK Subtraction
    }
  }

  weight *= g_scale_factor;

  if ( conf->get("reweight") == "true" || conf->get("vpt_reweight") == "true") {
    weight *= getReweight();

    if(conf->get("signal_region") == "all")
    {
      weight *= getReweight_allSR(SR);
    }
  }
  //L1 prefire weights
  if(!phys.isData() && (g_year == 2016 || g_year == 2017))
  {
      weight *= phys.weight_L1prefire(); 
  }

  if (conf->get("rwt_photon_eff") == "true" ){
    weight *= getEff(phys.gamma_pt().at(0), phys.gamma_eta().at(0));
  }


  if (conf->get("rwt_muon_eff") == "true"){
    weight *= getEff(phys.lep_pt().at(0), phys.lep_eta().at(0));
  }


 if ((! phys.isData()) ){
    if ( conf->get("event_type") == "dilepton" && (! MCTriggerEmulation)){

        //Ratio of data trigger efficiency to MC trigger efficiency
        //Slides from Marius hosted here : http://uaf-10.t2.ucsd.edu/~bsathian/ZMET_slides/ZEdge-20-01-02_MariusT.pdf
        if(g_year == 2016)
        {
            //simulated trigger efficiencies
            if (phys.hyp_type() == 0) weight *= 0.916/0.933; //ee
            if (phys.hyp_type() == 1) weight *= 0.949/0.980; //mumu
            if (phys.hyp_type() == 2) weight *= 0.890/0.917; //emu
        }
        else if(g_year == 2017)
        {

            if(phys.hyp_type() == 0) weight *= 0.906/0.955;
            if(phys.hyp_type() == 1) weight *= 0.873/0.947;
            if(phys.hyp_type() == 2) weight *= 0.858/0.928;
        }
        else if(g_year == 2018)
        {
            if(phys.hyp_type() == 0) weight *= 0.924/0.946;
            if(phys.hyp_type() == 1) weight *= 0.925/0.953;
            if(phys.hyp_type() == 2) weight *= 0.899/0.927;
        }
    }


    for (int i = 0; i < phys.nlep(); i++){
      weight*=phys.weightsf_lepid().at(i);
      weight*=phys.weightsf_lepiso().at(i);
      weight*=phys.weightsf_lepip().at(i);
      weight*=phys.weightsf_lepreco().at(i);
      weight*=phys.weightsf_lepconv().at(i);
    }

    if (conf->get("no_btag_sf") != "true"){
      //cout<<"Applying Btag Scale Factors"<<endl;
      weight *= phys.weight_btagsf();
    }

    if (conf->get("susy_mc") == "true" && conf->get("fastsim") == "true"){
      //double ISR_norm, btag_norm;
      if(conf->get("SUSY_Glu_LSP_scan") == "true"){
        ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm->GetYaxis()->FindBin(phys.mass_LSP()));
        //cout<<__LINE__<<endl;
        btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_norm->GetYaxis()->FindBin(phys.mass_LSP()));
      }
      else if (conf->get("SUSY_chi_scan") == "true"){
        //JUST READ FROM BIN 1 IN THE LSP ROW
        ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
        //cout<<__LINE__<<endl;
        btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
      }
      else{
        std::stringstream message;
        message<<"Can not get ISR or Btag normalization if SUSY_chi_scan or SUSY_Glu_LSP_scan are not set.";
        throw std::invalid_argument(message.str());
      }
      //cout<<__LINE__<<endl;
      weight *= phys.isr_weight(); //ISR scale factor
      weight *= ISR_norm;

      //cout<<__LINE__<<endl;

      weight *= btag_norm;

      //cout<<__LINE__<<endl;

      for (int i = 0; i < phys.nlep(); i++){
        weight *= phys.weightsf_lepid_FS().at(i); //Fast Sim Lepton ID
        weight *= phys.weightsf_lepiso_FS().at(i); //Fast Sim Lepton isolation
        weight *= phys.weightsf_lepip_FS().at(i); //Fast Sim Lepton impact parameter
        weight *= phys.weightsf_lepconv_FS().at(i); //Fast sim lepton conversion parameter
      }
    }
  }


  if (phys.isData() && phys.ngamma() > 0 && conf->get("event_type") == "photon"){
    weight *= getPrescaleWeight();
  }

  if (conf->get("tchihz_tchizz_weightfix") == "true"){
    double H_BR = (conf->get("tchihz_H_BR") == "" ) ? 0.5 : stod(conf->get("tchihz_H_BR"));
    double Z_BR = 1-H_BR;
    if(phys.evt_dataset().at(0).Contains("SMS-TChiHZ")){
      weight *= 2*H_BR*Z_BR; //twice the BR to HZ since we have both HZ and ZH.
    }
    else if (phys.evt_dataset().at(0).Contains("SMS-TChiZZ")){
      weight *= Z_BR*Z_BR;
    }
  }

  //TTBar ISR weight for 2016 alone
  if(g_year == 2016 && conf->get("Name").find("TTJets") != std::string::npos)
  {
      weight *= phys.isr_weight();
  }
  //
  //Fat jet stuff
  double fatjet_scale_factor = 1.0;
  int nGoodFatJets = 0;
  if(g_fatjet_indices.size() > 0 || g_fatjet_validation_indices.size() > 0)
  {
      nGoodFatJets = g_fatjet_indices.size() > 0 ? g_fatjet_indices.size() : g_fatjet_validation_indices.size();
      fatjet_scale_factor *= fatJetScaleFactor() * nGoodFatJets;
  }
  if(!phys.isData())
  {
    weight *= fatjet_scale_factor;
  }
    return weight;
}




double ZMETLooper::computeRsfof(int unc_mode)
{
    float Rsfof = 1.0;
    float rMuE_muon = 0.0;
    float rMuE_electron = 0.0;
    float *rMuE = nullptr;
    float Rt = 0.0;

     for(int i = 0; i < phys.nlep(); i++)
     {
        if(abs(phys.lep_pdgId().at(i)) == 11)
            rMuE = &rMuE_electron;
        else if(abs(phys.lep_pdgId().at(i)) == 13)
            rMuE = &rMuE_muon;
        else
        {
            cout<<"EMu event without electron and muon!, skipping calculation"<<endl;
        }

        if(rMuE != nullptr)
        {
            (*rMuE) = rMuEParameters["norm"] * (rMuEParameters["ptOffset"] + rMuEParameters["ptFalling"]/phys.lep_pt().at(i)) * (rMuEParameters["etaParabolaBase"] + (phys.lep_eta().at(i) < -1.6) * rMuEParameters["etaParabolaMinus"] * pow(phys.lep_eta().at(i)+1.6,2) + (phys.lep_eta().at(i) > 1.6) * rMuEParameters["etaParabolaPlus"] * pow(phys.lep_eta().at(i)-1.6,2) );
        }

        //Systematic errors
        if(unc_mode == 1)
        {
            (*rMuE) *= 1.05;
        }
        else if(unc_mode == -1)
        {
            (*rMuE) /= 1.05;
        }
        else if(unc_mode == 2)
        {
            (*rMuE) *= 1 + (0.05 * max((double)(110 - phys.lep_pt().at(i))/90 ,0.0)) ; 
        }
        else if(unc_mode == -2)
        {
            (*rMuE) /= 1 + (0.05 * max((double)(110 - phys.lep_pt().at(i))/90 , 0.0)) ; ;
        }
        else if(unc_mode == 3)
        {
            (*rMuE) *= 1 + 0.05* abs(phys.lep_eta().at(i))/2.4;
        }
        else if(unc_mode == -3)
        {
            (*rMuE) /= 1 + 0.05 * abs(phys.lep_eta().at(i))/2.4;
        }
    }
   //Errors will come later
   if(g_year == 2016)
        Rt = 1.049; 
    else if(g_year == 2017)
        Rt = 1.037;
    else if(g_year == 2018)
        Rt = 1.066;

    if(rMuE_muon != 0 && rMuE_electron != 0)
        Rsfof = (rMuE_electron + 1.0/rMuE_muon) * Rt * 0.5;
    else
        Rsfof = 1.0;

     return Rsfof;
}

double ZMETLooper::getPrescaleWeight(){
  if(g_year != 2016 && phys.HLT_Photon200() > 0 && phys.gamma_pt().at(0) > 210) return phys.HLT_Photon200();
  else if(!phys.HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0  && phys.gamma_pt().at(0) > 180)
  {
      if(g_year == 2016)
          return 1;
    else if(phys.gamma_pt().at(0) < 210) 
        return phys.HLT_Photon165_R9Id90_HE10_IsoM();
  }
  else if(phys.HLT_Photon165_HE10() > 0 && g_year == 2016) return 1;
  else if( !phys.HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 && phys.gamma_pt().at(0) < 180 ) return phys.HLT_Photon120_R9Id90_HE10_IsoM();
  else if( !phys.HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 && phys.gamma_pt().at(0) < 135  ) return phys.HLT_Photon90_R9Id90_HE10_IsoM();
  else if( !phys.HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 && phys.gamma_pt().at(0) < 105   ) return phys.HLT_Photon75_R9Id90_HE10_IsoM();
  else if( !phys.HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 && phys.gamma_pt().at(0) < 85    ) return phys.HLT_Photon50_R9Id90_HE10_IsoM();
  return 0;

}

//=============================
// Cuts
//=============================
bool ZMETLooper::passSRACuts()
{
    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_mt2 < 80)
    {
      return false;
    }
    if(g_njets < 2 || g_njets > 3)
    {
      return false;
    }
    if(g_ht < 500)
    {
      return false;
    }
    if(g_nBJetMedium35 > 0)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }
    return true;
}

bool ZMETLooper::passSRAbCuts()
{

    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_mt2 < 100)
    {
      return false;
    }
    if(g_njets < 2 || g_njets > 3)
    {
      return false;
    }
    if(g_ht < 200)
    {
      return false;
    }
    if(g_nBJetMedium35 < 1)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passVRACuts()
{
    if(g_dphi_metj1 > 0.4 && g_dphi_metj2 > 0.4)
    {
        return false;
    }
    if(g_njets < 2 || g_njets > 3)
    {
        return false;
    }
    if(g_ht < 300)
    {
        return false;
    }
    if(g_mt2 < 80)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }
    return true;
}


bool ZMETLooper::passSRBCuts()
{

    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_mt2 < 80)
    {
      return false;
    }
    if(g_njets < 4 || g_njets > 5)
    {
      return false;
    }
    if(g_ht < 500)
    {
      return false;
    }
    if(g_nBJetMedium35 > 0)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passSRBbCuts()
{

    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_mt2 < 100)
    {
      return false;
    }
    if(g_njets < 4 || g_njets > 5)
    {
      return false;
    }
    if(g_ht < 200)
    {
      return false;
    }
    if(g_nBJetMedium35 < 1)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passVRBCuts()
{
    if(g_dphi_metj1 > 0.4 && g_dphi_metj2 > 0.4)
    {
        return false;
    }
    if(g_njets < 4 || g_njets > 5)
    {
        return false;
    }
    if(g_ht < 300)
    {
        return false;
    }
    if(g_mt2 < 80)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}




bool ZMETLooper::passSRCCuts()
{

    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_njets < 6)
    {
      return false;
    }
    if(g_nBJetMedium35 > 0)
    {
      return false;
    }
    if(g_mt2 < 80)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passSRCbCuts()
{

    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_njets < 6)
    {
      return false;
    }
    if(g_nBJetMedium35 < 1)
    {
      return false;
    }
    if(g_mt2 < 100)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passVRCCuts()
{
    if(g_dphi_metj1 > 0.4 && g_dphi_metj2 > 0.4)
    {
        return false;
    }
    if(g_njets < 6)
    {
        return false;
    }
    if(g_mt2 < 80)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passSRVZCuts()
{
    if(phys.nFatJets() > 0 and (conf->get("2016_reproduce") != "true" and conf->get("SRVZ_strategy") == "A"))
    {
        return false;
    }
    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_njets < 2)
    {
      return false;
    }
//    if(g_nBJetMedium > 0)
    if((conf->get("resolved_jet_veto") == "loose" && g_nBJetLoose25 > 0) || (conf->get("resolved_jet_veto") == "medium" && g_nBJetMedium25 > 0))
    {
      return false;
    }
    if(g_mt2 < 80)
    {
      return false;
    }
    if(g_mjj_mindphi > 110)
    {
      return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;
}

bool ZMETLooper::passVRWZCuts()
{
   if(phys.nFatJets() > 0 and (conf->get("2016_reproduce") != "true" and conf->get("SRVZ_strategy") == "A"))
   {
       return false;
   }
   if(g_dphi_metj1 > 0.4 && g_dphi_metj2 > 0.4)
   {
       return false;
   }
   if(g_njets < 2)
   {
       return false;
   }
   if((conf->get("resolved_jet_veto") == "loose" && g_nBJetLoose25 > 0) || (conf->get("resolved_jet_veto") == "medium" && g_nBJetMedium25 > 0))
   {
       return false;
   }
   if(g_mt2 < 80)
   {
       return false;
   }
   if(g_mjj_mindphi > 110)
   {
       return false;
   }
   if(g_met < 50)
   {
        return false;
   }
   return true;
}

bool ZMETLooper::passSRVZBoostedCuts(int JMSMode,int JMRMode)
{
    g_fatjet_indices.clear();
    if(phys.njets() >= 2 && conf->get("SRVZ_strategy") == "B")
    {
        return false;
    }
    if(phys.nFatJets() < 1)
    {
      return false;
    }
    if((conf->get("boosted_jet_veto") == "loose" && g_nBJetLoose25 > 0) || (conf->get("boosted_jet_veto") == "medium" && g_nBJetMedium25 > 0))
    {
      return false;
    }
    if(g_dphi_met_fatjet < 0.8)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    for(size_t iJet = 0; iJet < phys.ak8jets_p4().size(); iJet++)
    {
      if(phys.ak8jets_tau2().at(iJet)/phys.ak8jets_tau1().at(iJet) > tau21WP())
      {
        continue;
      }
      //smearing stuff
      res->loadVariable("JetEta",phys.ak8jets_p4().at(iJet).eta());
      res->loadVariable("Rho",phys.rho());
      res->loadVariable("JetPt",phys.ak8jets_p4().at(iJet).pt());
      auto smearing = res->smear(JMRMode,g_year);
      cout<<"initial sdMass = "<<phys.ak8jets_softDropMass().at(iJet)<<" final sdMass = "<<phys.ak8jets_softDropMass().at(iJet) * fatJetJMSScaleFactor(JMSMode) * smearing[0]<<" smear value = "<<smearing[0]<<endl<<"jms mode = "<<JMSMode<<"jmr mode = "<<JMRMode<<endl;
      if(phys.ak8jets_softDropMass().at(iJet) * fatJetJMSScaleFactor(JMSMode) * smearing[0] < 65 || phys.ak8jets_softDropMass().at(iJet) * fatJetJMSScaleFactor(JMSMode) * smearing[0] > 105)
      {
        continue;
      }
    //indices of fat jets that pass selection. Needed for filling histograms
    g_fatjet_indices.push_back(iJet);
    }
    if(g_fatjet_indices.size() > 0)
        return true;
    return false;
}

bool ZMETLooper::passVRWZBoostedCuts()
{
    if(phys.njets() >= 2 && conf->get("SRVZ_strategy") == "B")
    {
        return false;
    }
    if(g_dphi_met_fatjet > 0.8)
    {
        return false;
    }
    if(phys.nFatJets() < 1)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }
 
//    if(phys.nBJetMedium() > 0)
    if((conf->get("boosted_jet_veto") == "loose" && g_nBJetLoose25 > 0) || (conf->get("boosted_jet_veto") == "medium" && g_nBJetMedium25 > 0))
    {
        return false;
    }
    for(size_t iJet = 0; iJet < phys.ak8jets_p4().size(); iJet++)
    {
      if(phys.ak8jets_tau2().at(iJet)/phys.ak8jets_tau1().at(iJet) > tau21WP())
      {
        continue;
      }
      if(phys.ak8jets_softDropMass().at(iJet) < 65 || phys.ak8jets_softDropMass().at(iJet) > 105)
      {
        continue;
      }
    //indices of fat jets that pass selection. Needed for filling histograms
    g_fatjet_validation_indices.push_back(iJet);
    }
    if(g_fatjet_validation_indices.size() > 0)
        return true;
    else
        return false;
}

bool ZMETLooper::passSRHZCuts()
{
    if(g_dphi_metj1 < 0.4)
    {
      return false;
    }
    if(g_dphi_metj2 < 0.4)
    {
      return false;
    }
    if(g_njets < 2)
    {
      return false;
    }
    if(g_nBJetMedium35 != 2)
    {
      return false;
    }
     if(g_mt2b < 200 && conf->get("event_type") != "photon")
     {
       return false;
     }
     if(g_mbb < 0 or g_mbb > 150)
     {
       return false;
     }
     if(g_met < 50)
     {
        return false;
     }

    return true;
}

bool ZMETLooper::passVRHZCuts()
{
    if(g_dphi_metj1 > 0.4 && g_dphi_metj2 > 0.4)
    {
        return false;
    }
    if(g_njets < 2)
    {
        return false;
    }
    if(g_nBJetMedium35 != 2)
    {
        return false;
    }
    if(g_mt2b < 200 && conf->get("event_type") != "photon")
    {
        return false;
    }
    if(g_mbb < 0 or g_mbb > 150)
    {
        return false;
    }
    if(g_met < 50)
    {
        return false;
    }

    return true;

}

bool ZMETLooper::passSignalRegionCuts(){

  //Njets Min Cut
  if (conf->get("Njets_min") != ""){
    if (g_njets < stod(conf->get("Njets_min"))){
      numEvents->Fill(34);
      if (printFail) cout<<phys.evt()<<" :Failed min jets cut"<<endl;
      return false;
    }
  }


  //Njets Max Cut
  if (conf->get("Njets_max") != ""){
    if (g_njets > stod(conf->get("Njets_max"))){
      numEvents->Fill(35);
      if (printFail) cout<<phys.evt()<<" :Failed max jets cut"<<endl;
      return false;
    }
  }

//Num Bottom jets Max Cut
  if (conf->get("NBjets_loose_max") != ""){
    if (phys.nBJetLoose25() > stod(conf->get("NBjets_loose_max"))){
      numEvents->Fill(37);
      if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }


  //Num Bottom jets Min Cut
  if (conf->get("NBjets_loose_min") != ""){
    if (g_nBJetLoose25 < stod(conf->get("NBjets_loose_min"))){
      numEvents->Fill(36);
      if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }

  //Num Bottom jets Min Cut
  if (conf->get("NBjets_min") != ""){
    if (g_nBJetMedium25 < stod(conf->get("NBjets_min"))){
      numEvents->Fill(36);
      if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }


  //Num Bottom jets Max Cut
  if (conf->get("NBjets_max") != ""){
    if (g_nBJetMedium25 > stod(conf->get("NBjets_max"))){
      numEvents->Fill(37);
      if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }

  //Num Bottom jets Max Cut

  if (conf->get("NFatJets_min") != "")
  {
      if(phys.nFatJets() < stod(conf->get("NFatJets_min")))
      {
          numEvents->Fill(78);
          if(printFail) cout<<phys.evt()<<" :Failed minimum fat jets cut"<<endl;
          return false;
      }
  }

  //Leading Jet/MET Phi min
  if (conf->get("dPhi_MET_j1") != ""){
    if (g_dphi_metj1 < stod(conf->get("dPhi_MET_j1"))){
      numEvents->Fill(38);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
      return false;
    }
  }

  if (conf->get("dPhi_MET_j1j2_max") != ""){
    if (g_dphi_metj1 > stod(conf->get("dPhi_MET_j1j2_max")) && g_dphi_metj2 > stod(conf->get("dPhi_MET_j1j2_max"))){
      numEvents->Fill(38);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
      return false;
    }
  }

  //Trailing Jet/MET Phi min
  if (conf->get("dPhi_MET_j2") != ""){
    if (g_dphi_metj2 < stod(conf->get("dPhi_MET_j2"))){
      numEvents->Fill(39);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 2 cut"<<endl;
      return false;
    }
  }

  if(conf->get("dPhi_MET_FatJet") != "" && conf->get("signal_region").find("Boosted") != std::string::npos)
  {
   if(g_dphi_met_fatjet < stod(conf->get("dPhi_MET_FatJet")))
   {
       numEvents->Fill(78);
       if(printFail) cout<<phys.evt()<<" :Failed dPhi_MET_FatJet cut"<<endl;
       return false;
   }
  }

  if(conf->get("dPhi_MET_FatJet_max") != "" && conf->get("signal_region").find("Boosted") != std::string::npos)
  {
   if(g_dphi_met_fatjet > stod(conf->get("dPhi_MET_FatJet_max")))
   {
       numEvents->Fill(78);
       if(printFail) cout<<phys.evt()<<" :Failed dPhi_MET_FatJet_max cut"<<endl;
       return false;
   }
  }

  //fill g_fatjet_indices if boosted region = true
  if(conf->get("boosted_histograms") == "true")
  {
        for(size_t iJet = 0; iJet < phys.ak8jets_p4().size(); iJet++)
        {
            if((conf->get("ak8_tau21_max") != "") && (phys.ak8jets_tau2().at(iJet)/phys.ak8jets_tau1().at(iJet) > stod(conf->get("ak8_tau21_max"))))
            {
                continue;
            }
            //indices of fat jets that pass selection. Needed for filling histograms
            g_fatjet_indices.push_back(iJet);
        }
  }

  //MT2b min
  if (conf->get("MT2b_min") != "" && conf->get("event_type") != "photon"){
    if (g_mt2b < stod(conf->get("MT2b_min"))){
      numEvents->Fill(40);
      if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }


  //MT2b min
  if (conf->get("MT2b_loose_min") != "" && conf->get("event_type") != "photon"){
    if (getMT2B() < stod(conf->get("MT2b_loose_min"))){
      numEvents->Fill(40);
      if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }

  //MT2 min
  if (conf->get("MT2_min") != ""){
    if (g_mt2 < stod(conf->get("MT2_min"))){
      numEvents->Fill(59);
      if (printFail) cout<<phys.evt()<<" :Failed MT2 cut"<<endl;
      return false;
    }


  }

if (conf->get("MT2_max") != ""){
    if (g_mt2 > stod(conf->get("MT2_max"))){
      numEvents->Fill(59);
      if (printFail) cout<<phys.evt()<<" :Failed MT2 cut"<<endl;
      return false;
    }


  }

  if(!phys.isData() and conf->get("genHT_min") != "")
  {
      if(phys.gen_ht() < stod(conf->get("genHT_min")))
          return false;
  }
  //HT min
  if (conf->get("HT_min") != ""){
  //if (printStats) { cout<<"ht: "<<g_ht<<" "; }
    if (g_ht < stod(conf->get("HT_min"))){
      numEvents->Fill(41);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT min cut"<<endl;
      return false;
    }
  }

  if (conf->get("HT_max") != ""){
    if (g_ht > stod(conf->get("HT_max"))){
      numEvents->Fill(41);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT max cut"<<endl;
      return false;
    }
  }

  //DiBottom mass difference from Higgs Mass
  if (conf->get("mbb_mh_diff") != ""){
    if (abs(g_mbb - 125) < stod(conf->get("mbb_mh_diff"))){
      numEvents->Fill(42);
      if (printFail) cout<<phys.evt()<<" :Failed sum mbb_mh diff cut"<<endl;
      return false;
    }
  }


  //Wierd ATLAS SR cut
  if (conf->get("sum_HT_pt_pt") != ""){
    double pt;
    //cout<<__LINE__<<endl;
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      pt = phys.gamma_pt().at(0);
    }
    else{
      pt = phys.lep_pt().at(0) + phys.lep_pt().at(1);
    }
    //cout<<__LINE__<<endl;
    if ( abs(g_ht + pt ) < stod(conf->get("sum_HT_pt_pt") ) ){
      numEvents->Fill(43);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT pt pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("lep1_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(0) < stod( conf->get("lep1_pt_min") )){
      numEvents->Fill(45);
      if (printFail) cout<<phys.evt()<<" :Failed lep1 min pt"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("lep2_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(1) < stod( conf->get("lep2_pt_min") )){
      numEvents->Fill(46);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("MET_min") != ""){
    double temp_met = g_met;
    if ( temp_met < stod( conf->get("MET_min") )){
      numEvents->Fill(56);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }


  //cout<<__LINE__<<endl;

  if (conf->get("MET_max") != ""){
    if ( g_met > stod( conf->get("MET_max") )){
      numEvents->Fill(57);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mbb_max") != ""){
    if ( g_mbb > stod( conf->get("Mbb_max") )){
      numEvents->Fill(58);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mbb_loose_max") != ""){
    if ( getMbb() > stod( conf->get("Mbb_loose_max") )){
      numEvents->Fill(58);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("MT_LepMET_min") != ""){
    if ( getMTLepMET() < stod( conf->get("MT_LepMET_min") ) ){
      numEvents->Fill(63);
      if (printFail) cout<<phys.evt()<<" :Failed MT from Lepton and MET min cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("Mjj_dphi_max") != ""){
    if ( g_mjj_mindphi > stod( conf->get("Mjj_dphi_max") ) ){
      numEvents->Fill(68);
      if (printFail) cout<<phys.evt()<<" :Failed Mjj cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("flavor") == "dimuon"){
    if ( phys.hyp_type() != 1){
      numEvents->Fill(73);
      if (printFail) cout<<phys.evt()<<" :Failed dimuon cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("flavor") == "dielectron"){
    if ( phys.hyp_type() != 0){
      numEvents->Fill(73);
      if (printFail) cout<<phys.evt()<<" :Failed dielectron cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("force_true_bjets") != ""){
    pair<int, int> b_index = getMostBlike();

    if ( abs(phys.jets_mcFlavour().at(b_index.first)) != 5 ){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }

    if ( abs(phys.jets_mcFlavour().at(b_index.second)) != 5 ){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("force_fake_bjets") != ""){
    pair<int, int> b_index = getMostBlike();

    if ( (abs(phys.jets_mcFlavour().at(b_index.first)) == 5) && (abs(phys.jets_mcFlavour().at(b_index.second)) == 5)){
      numEvents->Fill(67);
      if (printFail) cout<<phys.evt()<<" :Failed truth level bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  if (conf->get("signal_region") == "Legacy8TeV"){
    //cuts are leptons need to have |eta|<1.4 and 2jets for 100<=MET<150, 3jets for 150<=MET

    if (conf->get("event_type") == "dilepton"){
      if (abs(phys.lep_p4().at(0).eta()) > 1.4){
        return false;
      }
      if (abs(phys.lep_p4().at(1).eta()) > 1.4){
        return false;
      }
    }
    if (phys.met_pt() >= 150){
      if (phys.njets() < 2){
        return false;
      }
    }
    else{
      if (phys.njets() < 3){
        return false;
      }
    }
  }

  if (conf->get("has_tight_photon") == "true"){
    if( phys.ngamma() <  1 ) {
      numEvents->Fill(24);
      if (printFail) cout<<phys.evt()<<" :Failed at least 1 photon cut"<<endl;
      return false; // require at least 1 good photon
    }

    if( phys.gamma_pt().at(0) < 55 ) {
      numEvents->Fill(26);
      if (printFail) cout<<phys.evt()<<" :Failed pt < 55 photon cut"<<endl;
      return false; // photon pt > 55 GeV
    }

    if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
      numEvents->Fill(27);
      if (printFail) cout<<phys.evt()<<" :Failed gamm ain xition region photon cut"<<endl;
      return false; // veto xition region
    }

    if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
      numEvents->Fill(28);
      if (printFail) cout<<phys.evt()<<" :Failed gamma eta > 2.4 photon cut"<<endl;
      return false; // photon in EC or EB
    }

    if( phys.gamma_hOverE().at(0) > 0.1 ) {
      numEvents->Fill(29);
      if (printFail) cout<<phys.evt()<<" :Failed gamma hOverE photon cut"<<endl;
      return false; // H/E < 0.1
    }


    if( phys.matched_emf() < 0.7 ) {
      numEvents->Fill(30);
      if (printFail) cout<<phys.evt()<<" :Failed matched_emf photon cut"<<endl;
      return false; // jet neutral EM fraction cut
    }

    if( phys.elveto() ) {
      numEvents->Fill(32);
      if (printFail) cout<<phys.evt()<<" :Failed electron pixel veto photon cut"<<endl;
      return false; // veto pixel match
    }
  }

  //cout<<__LINE__<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes Signal Region Cuts"<<endl;
  return true;
}


/* The actual rare real MET cuts code - Replace the other one with this after producing new babies with new branch*/
 
bool ZMETLooper::passRareCuts()
{
    bool hasrealmet = true;
    bool hasrealzpair = true;
    float drThreshold = 0.1;

    std::vector<size_t> signal_lep_genMatch_indices;

    if(TString(conf->get("data_set")).Contains("VVV") || TString(conf->get("data_set")).Contains("TTZ") || TString(conf->get("data_set")).Contains("WZ") || TString(conf->get("data_set")).Contains("ZZ") || TString(conf->get("data_set")).Contains("GluGluToZZ"))
    {
        hasrealmet = false;
        hasrealzpair = false;

        for(size_t genind=0;genind < phys.genPart_motherId().size();genind++)
        {
            if((abs(phys.genPart_pdgId().at(genind)) == 12 || abs(phys.genPart_pdgId().at(genind)) == 14 || abs(phys.genPart_pdgId().at(genind)) == 16) && (abs(phys.genPart_motherId().at(genind)) == 24 || phys.genPart_motherId().at(genind) == 23) && (phys.genPart_status().at(genind) == 23 || phys.genPart_status().at(genind) == 1))
            {
                hasrealmet = true;
               if(printFail) cout<<phys.evt()<<" Found event containing real MET from neutrinos"<<endl;
            }

        }

        //Gen match the two signal leptons, and check their mother indices

        for(size_t lepidx = 0; lepidx < phys.lep_p4().size(); lepidx++)
        {
            float dr = drThreshold;
            int matchIdx = -1;
            for(size_t genidx = 0; genidx < phys.genLep_eta().size(); genidx++)
            {
                float temp_dr = sqrt(pow(phys.lep_p4().at(lepidx).eta() - phys.genLep_eta().at(genidx),2) + pow(phys.lep_p4().at(lepidx).phi() - phys.genLep_phi().at(genidx),2));
                if(temp_dr < dr)
                {
                    temp_dr = dr;
                    matchIdx = genidx;
                }
            }
            if(matchIdx >= 0 && abs(phys.genLep_motherId().at(matchIdx)) == 23)
            {
                signal_lep_genMatch_indices.push_back(matchIdx);
            }   
            else
            {
                if(printFail) cout<<phys.evt()<<" Signal lepton has no gen match!"<<endl;
                return false;
            }
        }

        if(signal_lep_genMatch_indices.size() >= 2 && phys.genLep_motherIndex().at(signal_lep_genMatch_indices[0]) == phys.genLep_motherIndex().at(signal_lep_genMatch_indices[1]))
        {
            if(printFail) cout<<phys.evt()<<" Found 2 signal leptons matched to the same gen Z"<<endl;
	   hasrealzpair = true;
        }
        
    }

    if(!hasrealmet)
    {
        numEvents->Fill(47);
        if (printFail) cout<<phys.evt()<<" :Failed Has Real MET Rare Cut"<<endl;
        return false;

    }
    else if(!hasrealzpair)
    {
        numEvents->Fill(48);
        if (printFail) cout<<phys.evt()<<" :Failed Has Real Z Rare Cut"<<endl;
        return false;
    }

    return true;
}
  
bool ZMETLooper::passSUSYSignalCuts(){
  if (conf->get("mass_chi") != ""){
    if (phys.mass_chi() != stod(conf->get("mass_chi"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass chi cut"<<endl;
      return false;
    }
  }


  if (conf->get("mass_gluino") != ""){
    if (phys.mass_gluino() != stod(conf->get("mass_gluino"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass gluino cut"<<endl;
      return false;
    }
  }


  if (conf->get("mass_LSP") != ""){
    if (phys.mass_LSP() != stod(conf->get("mass_LSP"))){
      numEvents->Fill(55);
      if (printFail) cout<<phys.evt()<<" :Failed mass LSP cut"<<endl;
      return false;
    }
  }
  return true;
}

bool ZMETLooper::isDuplicate(){
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    if (is_duplicate(id) ){
      ++nDuplicates;
      if (printFail) cout<<phys.evt()<<" :Is a duplicate"<<endl;
      return true;
    }
  }
  return false;
}


bool ZMETLooper::passMETFilters(){
  //For everyone
  
  if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()) {
    if (printFail) cout<<phys.evt()<<" :Failed EcalDeadCellTriggerPrimativeFilter cut"<<endl;
    return false;
  }
  if (!phys.Flag_HBHENoiseFilter()){
    if (printFail) cout<<phys.evt()<<" :Failed HBHENoiseFilter cut"<<endl;
    return false;
  }
  if (!phys.Flag_HBHEIsoNoiseFilter()){
    if (printFail) cout<<phys.evt()<<" :Failed HBHEIsoNoiseFilter cut"<<endl;
    return false;
  }
  if(!phys.Flag_badMuonFilter())
  {
      if(printFail) cout<<phys.evt()<<" :Failed BadMuonFilter"<<endl;
      return false;
  }

  if (!phys.Flag_goodVertices()) {
      if (printFail) cout<<phys.evt()<<" :Failed goodVerticies cut"<<endl;
    return false;
  }
  
  if (conf->get("susy_mc") != "true"){
    if (!phys.Flag_globalSuperTightHalo2016()){
      if (printFail) cout<<phys.evt()<<" :Failed globalSuperTightHalo2016 cut"<<endl;
      return false;
    }
  }
 
  if ( phys.isData() ) {
    if (!phys.Flag_eeBadScFilter()) {
      if (printFail) cout<<phys.evt()<<" :Failed eeBadScFilter cut"<<endl;
      return false;
    }
  }

  //if (printPass) cout<<phys.evt()<<": Passes MET Filters"<<endl;
  return true;
}

bool ZMETLooper::passBaseCut(){


  if (! (phys.evt_passgoodrunlist() > 0)){
    if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
    numEvents->Fill(8);
    return false;
  }

  if(conf->get("n_lep_veto") != ""){
      if(recompute_isotrack_flag)
      {
          isotrack_mt2 = compute_isotrack_mt2();
      }
      else
      {
          isotrack_mt2 = phys.nisoTrack_mt2();
      }
    if( (isotrack_mt2 + phys.nlep()) >= stod(conf->get("n_lep_veto"))){
        numEvents->Fill(54);
        if (printFail) cout<<phys.evt()<<" :Failed isotrack veto"<<endl;
        return false; //third lepton veto
    } 
    
        if(phys.nisoTrack_PFHad10_woverlaps() > 0)
        {
            numEvents->Fill(79);
                if(printFail) cout<<phys.evt()<<" :has hadron isotracks"<<endl;
            return false;
        }
    
    if (phys.nveto_leptons() >= 1){
      numEvents->Fill(66);
      if (printFail) cout<<phys.evt()<<" :Failed multi-lepton analysis lepton veto"<<endl;
      return false;
    }
/*    if (phys.nTaus20() >= 1){
      numEvents->Fill(75);
      if (printFail) cout<<phys.evt()<<" :Failed Tau veto"<<endl;
      return false;
    }*/
  }

  if(conf->get("multi_lep_veto") != ""){
    if (phys.nveto_leptons() >= 1){
      numEvents->Fill(66);
      if (printFail) cout<<phys.evt()<<" :Failed multi-lepton analysis lepton veto"<<endl;
      return false;
    }
    if (phys.nTaus20() >= 1){
      numEvents->Fill(75);
      if (printFail) cout<<phys.evt()<<" :Failed Tau veto"<<endl;
      return false;
    }
  }


  return true;
}



bool ZMETLooper::passETHDileptonDataCleanse(){
  /*Ensures events from the ee/mumu/emu dataset pass the trigger for that type of event and for ee and emu ensures they don't pass other triggers.*/
    /* Scheme
     * DoubleMuon - pass MuMu
     * DoubleEG - pass EE and not pass MuMu
     * MuonEG - pass EMu and not pass EE and MuMu
     */
  //Dilepton Data samples
  if ( (phys.isData()) && (TString(conf->get("data_set")).Contains("DoubleMuon") && TString(conf->get("data_set")).Contains("DoubleEG") && TString(conf->get("data_set")).Contains("MuonEG"))){
    //cout<<"Dilepton Data Event"<<endl;

    //ETH Trigger Cleansing
    if(phys.evt_dataset().at(0).Contains("MINIAOD") and !(phys.evt_dataset().at(0).Contains("MINIAODSIM"))){
      if (phys.evt_dataset().at(0).Contains("DoubleMuon") ){
        if( ! passMuonTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double muon dataset didn't pass double muon trigger"<<endl;
          return false;
        }
      }
      else if (phys.evt_dataset().at(0).Contains("DoubleEG")){
        if(! passElectronTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset didn't pass double electron trigger"<<endl;
          return false;
        }
        if(passMuonTriggers()) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset passed double muon trigger"<<endl;
          return false;
        }
      }
      else if (phys.evt_dataset().at(0).Contains("MuonEG")){
        if(! passEMuTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: EMu dataset didn't pass EMu trigger"<<endl;
          return false;
        }
        if( passElectronTriggers() ) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: EMu dataset dataset passed double electron trigger"<<endl;
          return false;
        }
        if(passMuonTriggers()) {
          //cout<<"skipped"<<endl;
          if (printFail) cout<<"ETH Trigger Cleansing: double electron dataset passed double muon trigger"<<endl;
          return false;
        }
      }
    }
  }

  return true;
}

bool ZMETLooper::passFileSelections(){
  /* Method which holds all the file specific selections, for instance cutting out the
  events with genht > 100 in the DY inclusive samples */



  //Zjets Monte Carlo samples
  if ( (! phys.isData()) && TString(conf->get("data_set")).Contains("ZMC") && conf->get("TemplatesClosure") == "true"){
    if( phys.evt_dataset().at(0).Contains("DYJetsToLL") and !phys.evt_dataset().at(0).Contains("HT")){
        //cut inclusive MC at 100 GeV gen HT
      if( phys.gen_ht() > 100 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(44);
        return false;
      }
  }
}


  //Photon MC samples
  if ( (! phys.isData()) && TString(conf->get("data_set")).Contains("GammaMC")){
    //cout<<"Photon MC event"<<endl;
    if( TString(currentFile->GetTitle()).Contains("gjetsht40") ||  TString(currentFile->GetTitle()).Contains("gjetsht100") ){
      if( abs(phys.gen_ht() - g_ht) > 300 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(69);
        return false;
      }
    }
  }

  //WJets cocktail for inclusive photon sample and Electroweak Subtraction
  if ( (TString(conf->get("data_set")).Contains("WGamma") ||TString(conf->get("data_set")).Contains("WJets") || TString(conf->get("data_set")).Contains("EWKSub")) && !TString(conf->get("Name")).Contains("no-overlap")){

    //Inclusive GenHT Cut
    if(phys.evt_dataset().at(0).Contains("WJets") and not phys.evt_dataset().at(0).Contains("HT")){
     // cout<<"File: "<<currentFile->GetTitle()<<" with gen_ht: "<<phys.gen_ht()<<endl;
      if( phys.gen_ht() > 100 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(44);
        return false;
      }
    }

    //Remove overlap between WGammaJets and WJets
    if(conf->get("data_set") == "WJets"){ //WJets
      if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) == 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }
    }
    else if (conf->get("data_set") == "WGamma"){ //WGammaJets
      if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) != 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }
    }

    //remove the events with less than 40 gen pt
    if ( TString(currentFile->GetTitle()).Contains("wgjets_incl_mgmlm") && phys.ngamma() > 0){
      //First get best photon match
      int bestMatch = -1;
      float bestDR = 0.1;
      double eta = phys.gamma_eta().at(0);
      double phi = phys.gamma_phi().at(0);
      for(unsigned int iGen = 0; iGen < phys.genPart_pdgId().size(); iGen++){
        if ((phys.genPart_pdgId().at(iGen) != 22) /*&& (abs(phys.genPart_pdgId().at(iGen)) != 11)*/ ) continue; // accept gen photons
        if ((fabs(phys.genPart_motherId().at(iGen)) > 24) && (fabs (phys.genPart_motherId().at(iGen)) != 2212) ) continue; // don't want stuff from pions etc
        if (phys.genPart_status().at(iGen) != 1  ) continue;
        if (fabs(eta - phys.genPart_eta().at(iGen)) > 0.1 ) continue;
        float thisDR = sqrt(pow(eta-phys.genPart_eta().at(iGen), 2) + pow(phi-phys.genPart_phi().at(iGen), 2)); //DeltaR( cms3.genps_p4() .at(iGen).eta(), eta, cms3.genps_p4().at(iGen).phi(), phi);
        if (thisDR < bestDR) {
          bestMatch = iGen;
          bestDR=thisDR;
        }
      }
      if (bestMatch < 0){
        numEvents->Fill(72);
        return false;
      }
      else if (phys.genPart_pt().at(bestMatch) > 40){
        numEvents->Fill(72);
        return false;
      }
    }
  }


/* if((TString(conf->get("data_set")).Contains("ttbarGamma") || TString(conf->get("data_set")).Contains("ttbarJets")) && !TString(conf->get("Name")).Contains("no_overlap"))
 {
    //Overlap removal between TTGamma and TTJets
    //
    if(TString(conf->get("data_set")).Contains("ttbarGamma"))
    {
        if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) != 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }

    }
    else if(TString(conf->get("data_set")).Contains("ttbarJets"))
    {
        if( phys.ngamma() > 0 && phys.gamma_genIsPromptFinalState().at(0) == 1 ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        return false;
      }

    }

 }*/

//old overlap removal stuff here

  if ( TString(conf->get("data_set")).Contains("FSMC-TTBar-NoPromptGamma") ){

    //Remove prompt photons from TTBar
    if( TString(currentFile->GetTitle()).Contains("ttbar") ){
      if( phys.ngamma() > 0 && (phys.gamma_genIsPromptFinalState().at(0) == 1 && phys.gamma_mcMatchId().at(0) == 22)) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        num_events_veto_ttbar++;
        return false;
      }
    }
  }

  if ( TString(conf->get("data_set")).Contains("FSMC-TTGamma-NoNonPromptGamma") ){

    //Remove Non-prompt from TTGamma
    if ( TString(currentFile->GetTitle()).Contains("ttgamma_incl_amcnlo") ){
      if( phys.ngamma() > 0 && (phys.gamma_genIsPromptFinalState().at(0) != 1 || phys.gamma_mcMatchId().at(0) != 22 ) ) {
        //cout<<"skipped"<<endl;
        numEvents->Fill(64);
        num_events_veto_ttgamma++;
        return false;
      }
    }
  }

  return true;
}

//=============================
// Setup
//=============================

void ZMETLooper::setupGlobals(){
  if ( conf->get("uncertainty_mode") == "JES_up" ){
    g_dphi_metj1 = phys.dphi_metj1_up();
    g_dphi_metj2 = phys.dphi_metj2_up();
    g_njets = phys.njets_up();
    g_mbb = phys.mbb_csv_up();
    g_mjj_mindphi = phys.mjj_mindphi_up();
    g_nBJetMedium35 = phys.nBJetMedium35_up();
    g_nBJetMedium25 = phys.nBJetMedium25_up();
    g_nBJetLoose35 = phys.nBJetLoose35_up();
    g_nBJetLoose25 = phys.nBJetLoose25_up();
    g_met = phys.met_T1CHS_miniAOD_CORE_up_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_up_phi();
    g_mt2 = phys.mt2_up();
    g_mt2b = phys.mt2b_up();
    g_ht = phys.ht_up();
    g_jets_p4 = phys.jets_up_p4();
    g_jets_medb_p4 = phys.jets_medb_up_p4();
    g_jets_csv = phys.jets_up_csv();
  }
  else if (conf->get("uncertainty_mode") == "JES_dn"){
    g_dphi_metj1 = phys.dphi_metj1_dn();
    g_dphi_metj2 = phys.dphi_metj2_dn();
    g_njets = phys.njets_dn();
    g_mbb = phys.mbb_csv_dn();
    g_mjj_mindphi = phys.mjj_mindphi_dn();
    g_nBJetMedium35 = phys.nBJetMedium35_dn();
    g_nBJetMedium25 = phys.nBJetMedium25_dn();
    g_nBJetLoose35 = phys.nBJetLoose35_dn();
    g_nBJetLoose25 = phys.nBJetLoose25_dn();
    g_met = phys.met_T1CHS_miniAOD_CORE_dn_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_dn_phi();
    g_mt2 = phys.mt2_dn();
    g_mt2b = phys.mt2b_dn();
    g_ht = phys.ht_dn();

    g_jets_p4 = phys.jets_dn_p4();
    g_jets_medb_p4 = phys.jets_medb_dn_p4();
    g_jets_csv = phys.jets_dn_csv();
  }
  else if (conf->get("uncertainty_mode") == "GenMet"){
    g_dphi_metj1 = phys.dphi_genmetj1();
    g_dphi_metj2 = phys.dphi_genmetj2();
    g_met = phys.met_genPt();
    g_met_phi = phys.met_genPhi();
    g_mt2 = phys.mt2_genmet();
    g_mt2b = phys.mt2b_genmet();

    g_mjj_mindphi = phys.mjj_mindphi();
    g_mbb = phys.mbb_csv();
    g_nBJetMedium35 = phys.nBJetMedium35();
    g_nBJetMedium25 = phys.nBJetMedium25();
    g_nBJetLoose35 = phys.nBJetLoose35();
    g_nBJetLoose25 = phys.nBJetLoose25();
    g_njets = phys.njets();
    g_ht = phys.ht();
    g_jets_p4 = phys.jets_p4();
    g_jets_medb_p4 = phys.jets_medb_p4();
    g_jets_csv = phys.jets_csv();
  }
  else{
    g_dphi_metj1 = phys.dphi_metj1();
    g_dphi_metj2 = phys.dphi_metj2();
    g_njets = phys.njets();
    g_mbb = phys.mbb_csv();
    g_mjj_mindphi = phys.mjj_mindphi();
    g_nBJetMedium35 = phys.nBJetMedium35();
    g_nBJetMedium25 = phys.nBJetMedium25();
    g_nBJetLoose35 = phys.nBJetLoose35();
    g_nBJetLoose25 = phys.nBJetLoose25();
    g_met = phys.met_T1CHS_miniAOD_CORE_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_phi();
    g_mt2 = phys.mt2();
    g_mt2b = phys.mt2b();
    g_ht = phys.ht();
    g_mht = computeMht().pt();
    g_mht_phi = computeMht().phi();
    g_jets_p4 = phys.jets_p4();
    g_jets_medb_p4 = phys.jets_medb_p4();
    g_jets_csv = phys.jets_csv();
  }
  //fat jet dphi
  if(phys.nFatJets() > 0)
  {
    g_dphi_met_fatjet = acos(cos(phys.ak8jets_p4().at(0).phi() - phys.met_T1CHS_miniAOD_CORE_phi()));
  }
  //fat jet setup
  g_fatjet_indices.clear();
  g_fatjet_validation_indices.clear();
  g_fatjet_inclusive_indices.clear();
}

void ZMETLooper::updateSUSYBtagISRNorms(){
  /*Loads the proper TH2 for the given SUSY sample which contains the BTag and ISR weights if running SUSY MC.*/
  //--------------------------------------------------------
  // 2D SUSY Scan ISR and BTag SF normalization Histograms
  //--------------------------------------------------------
  if(conf->get("susy_mc") == "true" && conf->get("fastsim") == "true"){
    cout<<"Updating normalization weights for ISR and Btag Scale Factors."<<endl;
    //cout<<__LINE__<<endl;
    if (phys.evt_dataset().at(0).Contains("T5ZZ")){
      cout<<"Using auxFiles/nsig_weights_t5zz.root for ISR/Btag avg weights"<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_t5zz.root", "READ");
    }
    else if (phys.evt_dataset().at(0).Contains("TChiWZ-Ext")){
      cout<<"Using auxFiles/nsig_weights_tchiwz_ext.root for ISR/Btag avg weights"<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchiwz_ext.root", "READ");
    }
    else if (phys.evt_dataset().at(0).Contains("TChiWZ")){
      cout<<"Using auxFiles/nsig_weights_tchiwz.root for ISR/Btag avg weights"<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchiwz.root", "READ");
    }
    else if (phys.evt_dataset().at(0).Contains("TChiHZ")){
      cout<<"Using auxFiles/nsig_weights_tchihz.root for ISR/Btag avg weights"<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchihz.root", "READ");
    }
    else if (phys.evt_dataset().at(0).Contains("TChiZZ")){
      cout<<"Using auxFiles/nsig_weights_tchizz.root for ISR/Btag avg weights"<<endl;
      g_SUSYsf_norm_file = TFile::Open("auxFiles/nsig_weights_tchizz.root", "READ");
    }
    else {
      //cout<<__LINE__<<endl;
      std::stringstream message;
      message<<"Can not update normalization weights file for file: "<<currentFile->GetTitle()<<", no external hist file configured for it.";
      throw std::invalid_argument(message.str());
    }
    //cout<<__LINE__<<endl;
    int year = getYear();
    g_isr_norm = (TH2D*)g_SUSYsf_norm_file->Get(TString::Format("h_avg_weight_isr_%d",year))->Clone("h_isr_norm");
    //cout<<__LINE__<<endl;
    g_isr_norm_up = (TH2D*)g_SUSYsf_norm_file->Get(TString::Format("h_avg_weight_isr_UP_%d",year))->Clone("h_isr_norm_up");
    //cout<<__LINE__<<endl;
    g_btagsf_norm = (TH2D*)g_SUSYsf_norm_file->Get(TString::Format("h_avg_weight_btagsf_%d",year))->Clone("g_btagsf_norm");
    //cout<<__LINE__<<endl;
    g_btagsf_light_norm_up = (TH2D*)g_SUSYsf_norm_file->Get(TString::Format("h_avg_weight_btagsf_light_UP_%d",year))->Clone("g_btagsf_light_norm_up");
    //cout<<__LINE__<<endl;
    g_btagsf_heavy_norm_up = (TH2D*)g_SUSYsf_norm_file->Get(TString::Format("h_avg_weight_btagsf_heavy_UP_%d",year))->Clone("g_btagsf_heavy_norm_up");
    //cout<<__LINE__<<endl;


    g_isr_norm->SetDirectory(rootdir);
    g_isr_norm_up->SetDirectory(rootdir);
    g_btagsf_norm->SetDirectory(rootdir);
    g_btagsf_light_norm_up->SetDirectory(rootdir);
    g_btagsf_heavy_norm_up->SetDirectory(rootdir);


    g_SUSYsf_norm_file->Close();
  }
}

void ZMETLooper::setupExternal(TString savePath){
  /*Loads Pt reweighting histograms, pileup reweighting hists, and efficiency hists (which are no longer used really). Also sets up goodrun list*/

  //Set up manual vertex reweighting.

 //set up year

//  g_year = conf->get("year")!="" ? stoi(conf->get("year")) : 2017;
   if(year_fromCommandLine > 0)
   {
     g_year = year_fromCommandLine;
   }
   else
   {
     g_year = 2017;
   }

  if( conf->get("reweight") == "true" ){
    readyReweightHists();
  }
  if( conf->get("vpt_reweight") == "true" ){
    if(conf->get("signal_region") == "all")
    {
      readyVPTReweight_allSR(savePath);
    }
    else
    {
      readyVPTReweight(savePath);
    }
  }

  if(conf->get("pileup_reweight") == "true" and conf->get("data") == "false"){
    if(g_year == 2016)
    {
        cout<<"Pileup reweighting with puWeight2016.root"<<endl;
        g_pileup_hist_file = TFile::Open("auxFiles/puWeight2016.root", "READ");
    }
    else if(g_year == 2017)
    {
        cout<<"Pileup reweighting with puWeight2017.root"<<endl;
        g_pileup_hist_file = TFile::Open("auxFiles/puWeight2017.root","READ");
    }
    else if(g_year == 2018)
    {
        cout<<"Pileup reweighting with puWeight2018.root"<<endl;
        g_pileup_hist_file = TFile::Open("auxFiles/puWeight2018.root","READ");
    }

    //cout<<__LINE__<<endl;
    g_pileup_hist = (TH1D*)g_pileup_hist_file->Get("pileupWeight")->Clone("h_pileup_weight");
    //cout<<__LINE__<<endl;
    g_pileup_hist->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_pileup_hist_file->Close();
  }

  //cout<<__LINE__<<endl;
  //cout<<__LINE__<<endl;

  if( conf->get("rwt_photon_eff") == "true" ){
    cout<<"Reweighting for Effeciency with trigeff_Photon165_zmet2016.root"<<endl;
    TFile weight_eff_file("auxFiles/trigeff_Photon165_zmet2016.root", "READ");

    //barrel
    g_pt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_eb_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_pt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_pt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_ee_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_pt_eff_endcap->SetDirectory(rootdir);

    weight_eff_file.Close();
  }

  if( conf->get("rwt_muon_eff") == "true" ){
    cout<<"Reweighting for single muon trigger effeciency"<<endl;
    TFile weight_eff_file("/home/users/cwelke/analysis/CMSSW_8_0_22/V08-22-05/ZMET2015/makePlots/trigeff_Muon_pt_2016_withH.root", "READ");

    //barrel
    g_pt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_denom_eb_ele27WPLoose_clone")->Clone("g_pt_eff_barrel");
    g_pt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_pt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_denom_ee_ele27WPLoose_clone")->Clone("g_pt_eff_barrel");
    g_pt_eff_endcap->SetDirectory(rootdir);

    weight_eff_file.Close();
  }

 }


int ZMETLooper::ScanChain( TChain* chain, ConfigParser *configuration, bool fast/* = true*/, int nEvents/* = -1*/) {
  /* Runs through baby files and makes histogram files.

  Inputs:
  chain -- contains the files to make the histograms from,
  configuration -- pointer to the configuration object
  */
  //cout<<__LINE__<<endl;
  //Set Global Vars
  conf=configuration;
  //cout<<__LINE__<<endl;
  g_sample_name=conf->get("Name");

  if (conf->get("MCTriggerEmulation") != ""){
    if (conf->get("MCTriggerEmulation") == "true"){
      cout<<"Manually setting MC trigger emulation to true"<<endl;
      MCTriggerEmulation=true;
    }
    else if (conf->get("MCTriggerEmulation") == "false"){
      cout<<"Manually setting MC trigger emulation to false"<<endl;
      MCTriggerEmulation=false;
    }
  }
  
  TString savePath; 
  if(conf->get("auto_scale") == "true")
  {
      savePath = getOutputDir(conf,"allhist",year_fromCommandLine);
  }
  else savePath = getOutputDir(conf, "hist");
  ofstream files_log;
  files_log.open((savePath+TString(g_sample_name+"_files.log")).Data());
  //cout<<__LINE__<<endl;
  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  if (conf->get("scale_factor") != ""){
    g_scale_factor*=stod(conf->get("scale_factor"));
  }

//===========================================
// Define Histograms
//===========================================

  clear_list(); //Event duplicate removal clear list

  cout<<"Opening file "<<TString(savePath+conf->get("Name")+".root")<<endl;
  std::string folderCreation = "mkdir -p " + std::string(savePath.Data());
  system(folderCreation.c_str());
  TFile * output = new TFile(TString(savePath+conf->get("Name")+".root"), "RECREATE");
  output->cd();

 //Special snowflake histogram - Cannot go into the main map
 if(conf->get("dilep_control_region") == "true" and conf->get("data") == "true")
 {
     ee_numEvents = new TH1I("ee_numEvents","ee_numEvents",80,0,80);
     ee_numEvents->SetDirectory(rootdir);
     mumu_numEvents = new TH1I("mumu_numEvents","mumu_numEvents",80,0,80);
     mumu_numEvents->SetDirectory(rootdir);
     emu_numEvents = new TH1I("emu_numEvents","emu_numEvents",80,0,80);
     emu_numEvents->SetDirectory(rootdir);
 }

  else {
    numEvents = new TH1I("numEvents", "Number of events in "+g_sample_name, 80, 0, 80);
    numEvents->SetDirectory(rootdir);
 }




  //cout<<__LINE__<<endl;
//===========================================
// Setup Stuff Pulled From External Files
//===========================================

  setupExternal(savePath);

  //cout<<__LINE__<<endl;
  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  int eventCount=0;
  if(conf->get("dil_flavor") == "all")
  {
      ee_eventCount = 0;
      mumu_eventCount = 0;
      emu_eventCount = 0;
  }
  cout<<"DATASET: "<<conf->get("data_set")<<endl;
  if( nEvents >= 0 ) nEventsChain = nEvents;
  //cout<<__LINE__<<endl;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  //cout<<__LINE__<<endl;
  TIter fileIter(listOfFiles);
  //cout<<__LINE__<<endl;
  
  //Load JSON file for rMuE
  std::string param_file_name;
  fstream param_file;
  if(conf->get("R_sfof_from_json") == "true")
  {
    if(g_year == 2016)
    {
        param_file_name = "auxFiles/rMuE_correctionParameters_ZPeakControl_Run2016_36fb.json";
    }
    else if(g_year == 2017)
    {
        param_file_name = "auxFiles/rMuE_correctionParameters_ZPeakControl_Run2017_42fb.json";
    }
    else if(g_year == 2018)
    {
        param_file_name = "auxFiles/rMuE_correctionParameters_ZPeakControl_Run2018_60fb.json";
    }
    
    rMuEParameters = extractrMuEParamsFromJSON(param_file_name); 
  }
//===========================================
// File Loop
//===========================================
    
    unsigned int HEM_fracNum = 1286, HEM_fracDen = 1961;
    while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile *file = TFile::Open( currentFile->GetTitle() );
    //cout<<__LINE__<<endl;
    TTree *tree = (TTree*)file->Get("t");
    //cout<<__LINE__<<endl;
    if(fast) TTreeCache::SetLearnEntries(10); //What does this do?
    //cout<<__LINE__<<endl;
    if(fast) tree->SetCacheSize(128*1024*1024); //What does this do?
    //cout<<__LINE__<<endl;
    phys.Init(tree); //Loads in all the branches
    res = new JetResolution;
        if(g_year == 2016)
    {
        res->loadResolutionFile("JetResolution/Spring16_25nsV10_MC_PtResolution_AK8PFPuppi.txt");
    }
    else if(g_year == 2017)
    {
        res->loadResolutionFile("JetResolution/Fall17_V3b_MC_PtResolution_AK8PFPuppi.txt");
    }
    else if(g_year == 2018)
    {
        res->loadResolutionFile("JetResolution/Autumn18_V7b_MC_PtResolution_AK8PFPuppi.txt");
    }
    //cout<<__LINE__<<endl;
    //cout<<__LINE__<<endl;
    files_log<<"Running over new file: "<<currentFile->GetTitle()<<endl;
    cout<<"Running over new file: "<<currentFile->GetTitle()<<endl;

    if(conf->get("dilep_control_region") == "true" and phys.isData())
    {
        if(TString(currentFile->GetTitle()).Contains("DoubleEG") || TString(currentFile->GetTitle()).Contains("EGamma"))
            numEvents = ee_numEvents;
        else if(TString(currentFile->GetTitle()).Contains("DoubleMuon"))
            numEvents = mumu_numEvents;
        else if(TString(currentFile->GetTitle()).Contains("MuonEG"))
            numEvents = emu_numEvents;
    }

    if (conf->get("susy_mc") == "true" && conf->get("fastsim") == "true") updateSUSYBtagISRNorms();
//===========================================
// Loop over Events in current file
//===========================================
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      numEvents->Fill(0);
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      phys.GetEntry(event);
      ++nEventsTotal;
      //cout<<__LINE__<<endl;
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );
      setupGlobals();

      if (conf->get("ETH_Cleanse") != "false"){
          if (phys.isData() && conf->get("event_type") == "dilepton" && (! passETHDileptonDataCleanse()) ){
          //cout<<"Failed ETH Dilepton Data Cleanse"<<endl;
          continue;
        }
      }

      if ( isDuplicate() ){
        numEvents->Fill(23);
        continue;
      } // check for duplicates

      if (! passFileSelections() ){
        continue;
      }

      if (! passBaseCut()){
        continue;
      }// Base Cut

      if (! hasGoodEvent()){
        continue; // Event Type Specific Cuts
      }

      if (conf->get("rare_real_MET_Z") == "true"){
        if ( ! passRareCuts() ){
          continue;
        } //Rare Sample Selections
      }

      //HEM1516 issue
      if(g_year == 2018 && ((phys.isData() && phys.run() >= 319077) || (!phys.isData() && event % HEM_fracDen < HEM_fracNum )))
      {
          if(!passHEM1516Veto())
          {
              continue;
          }
      }
              

      if (conf->get("susy_mc") == "true"){
        if (! passSUSYSignalCuts()){
          continue;
        }
      }

      if (conf->get("do_met_filters") != "false")
      {
        if (! passMETFilters()) continue; ///met filters
      }
      commonHistPrefix.clear();


       if(conf->get("signal_region") != "all")
       {
            if (! passSignalRegionCuts()){

                continue; // Signal Region Cuts
            }
            fillallHistograms();
        }
        else
        {
        //hack to ensure event gets checked for all regions
            bool flag = false;

            //SUSY MC special stuff
            if(conf->get("susy_mc") == "true")
            {
                if(conf->get("Name").find("T5ZZ") != std::string::npos)
                {
                   flag = passStrongSRCuts();
                }
                else if(conf->get("Name").find("TChi") != std::string::npos)
                {
                    flag = passEWKSRCuts();
                }
            }

            else
            {
                flag = passStrongSRCuts();
                flag = passEWKSRCuts();
                flag = passStrongVRCuts();
                flag = passEWKVRCuts();
                flag = passInclusiveCuts();
            }

            if(flag == false)
            {
            continue;
            }
        }
      

      if(conf->get("printEvtList") == "true"){
        cout<<"evt: "<<phys.evt()<<" run: "<<phys.run()<<" lumi: "<<phys.lumi()<<" scale1fb: "<<phys.evt_scale1fb()<<" weight: "<<weight<<" extra_weight: "<< weight/phys.evt_scale1fb() <<endl;
      }

      eventCount++;

      if(conf->get("dil_flavor") == "all")
      {
          if(dil_flavor == 0)
              ee_eventCount++;
          else if(dil_flavor == 1)
              mumu_eventCount++;
          else if(dil_flavor == 2)
              emu_eventCount++;
      }

      eventCount++;

    }
    // Clean Up
    delete tree;
    file->Close();
    delete res;
 }


  cout<<"Num events passed: "<<eventCount<<endl;

  if(conf->get("dil_flavor") == "all")
  {
      cout<<"Num ee events passed: "<<ee_eventCount<<endl;
      cout<<"Num mumu events passed: "<<mumu_eventCount<<endl;
      cout<<"Num emu events passed: "<<emu_eventCount<<endl;
  }
  files_log<<"Num events passed: "<<eventCount<<endl;
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

//=======================================
// Write Out Histos
//=======================================
  output->cd();

  if(conf->get("dilep_control_region") == "true" and conf->get("data") == "true")
  {
      ee_numEvents->Write();
      mumu_numEvents->Write();
      emu_numEvents->Write();
  }
  else
  {
    numEvents->Write();
  }

  for(auto &it:allHistos)
  {
      it.second->Write();
  }
  for(auto &it:all2DHistos)
  {
      it.second->Write();
  }
  for(auto &it:allSignal2DHistos)
  {
      it.second->Write();
  }
  for(auto &it:allSignal3DHistos)
  {
      it.second->Write();
  }
  //close output file
  output->Write();
  output->Close();
  g_reweight_pairs.clear();
  files_log.close();
  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << num_events_veto_ttbar << " Events Vetod from TTBar" << endl;
  cout << num_events_veto_ttgamma << " Events Vetod from TTGamma" << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << nDuplicates << " Duplicates" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;

  //====================
  // Clean Everything Up
  //====================
  delete bmark;
  
  return 0;
}


void ZMETLooper::fillGluLSPHists(std::string prefix)
{
    if(conf->get("data_set").find("TChiWZ") != std::string::npos)
    {
       
        n_gluino_bins = &n_gluino_bins_tchiwz;
        n_lsp_bins = &n_lsp_bins_tchiwz;
        gluino_bins = gluino_bins_tchiwz;
        lsp_bins = lsp_bins_tchiwz;

        if(prefix.find("SRHZ") != std::string::npos)
        {
            n_met_bins = &n_met_bins_tchiwz_SRHZ;
            met_bins = met_bins_tchiwz_SRHZ;
        }

        else if(prefix.find("SRVZBoosted") != std::string::npos)
        {
            n_met_bins = &n_met_bins_tchiwz_boosted;
            met_bins = met_bins_tchiwz_boosted;
        }
        else
        {
            n_met_bins = &n_met_bins_tchiwz_resolved;
            met_bins = met_bins_tchiwz_resolved;
        } 
    }
    else if(conf->get("data_set") == "T5ZZ")
    {
        n_gluino_bins = &n_gluino_bins_t5zznat;
        n_lsp_bins = &n_lsp_bins_t5zznat;
        gluino_bins = gluino_bins_t5zznat;
        lsp_bins = lsp_bins_t5zznat;

        if(conf->get("fastsim_2016_binning") != "true")
        {
            if(prefix.find("SRC") != std::string::npos)
            {
                n_met_bins = &n_met_bins_t5zznat_SRC;
                met_bins = met_bins_t5zznat_SRC;
            }
            else
            { 
                n_met_bins = &n_met_bins_t5zznat;
                met_bins = met_bins_t5zznat;
            }
        }
        else
        {
            if(prefix.find("SRC") != std::string::npos)
            {
                n_met_bins = &n_met_bins_t5zznat_2016_SRC;
                met_bins = met_bins_t5zznat_2016_SRC;
            }
            else
            { 
                n_met_bins = &n_met_bins_t5zznat_2016;
                met_bins = met_bins_t5zznat_2016;
            }
        }
    }

    fill2DHistograms(prefix+"mass_counts",phys.mass_gluino(),phys.mass_LSP(),weight,all2DHistos,"(x,y) = (mass_gluino,mass_LSP)",*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);

    fill3DHistograms(prefix+"susy_type1MET_counts",g_met,phys.mass_gluino(),phys.mass_LSP(),weight,allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type1MET for"+g_sample_name, *n_met_bins, met_bins, *n_gluino_bins, gluino_bins, *n_lsp_bins, lsp_bins,rootdir);
    fill3DHistograms(prefix+"susy_type1MET_nowt",g_met,phys.mass_gluino(),phys.mass_LSP(),1,allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type1MET with no event weights for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);
    
    double tau21_weight_up, tau21_weight_down;
    double tau21_central = fatJetScaleFactor(0);
    double tau21_unc;
    if(prefix.find("SRVZBoosted") != std::string::npos)
    {
        tau21_unc = fatJetScaleFactor(1);

        //do the pt dependent uncertainty right here
        for(int iJet = 0; iJet < phys.ak8jets_p4().size(); iJet++)
        {
            tau21_unc = sqrt(tau21_unc * tau21_unc + fatJetPtError(phys.ak8jets_p4().at(iJet).pt(),g_year,tau21) * fatJetPtError(phys.ak8jets_p4().at(iJet).pt(),g_year,tau21));
        }

        tau21_weight_up = weight * (1 + tau21_unc/tau21_central);
        tau21_weight_down = weight * (1 - tau21_unc/tau21_central);

        fill3DHistograms(prefix+"susy_type1MET_tau21_up",g_met,phys.mass_gluino(),phys.mass_LSP(),tau21_weight_up,allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type1MET with tau21 up weights for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);

        fill3DHistograms(prefix+"susy_type1MET_tau21_down",g_met,phys.mass_gluino(),phys.mass_LSP(),tau21_weight_down,allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type1MET with tau21 down weights for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);



        for(auto &i:g_fatjet_indices)
        {
            fill3DHistograms(prefix+"susy_tau21",phys.ak8jets_tau2().at(i)/phys.ak8jets_tau1().at(i),phys.mass_gluino(),phys.mass_LSP(),weight,allSignal3DHistos,"",1000,0,1,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);
        }
    }

   
    if(conf->get("data_set") != "TChiWZfullsim")
    {
    	ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm->GetYaxis()->FindBin(phys.mass_LSP()));
    	ISR_norm_up=1./g_isr_norm_up->GetBinContent(g_isr_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_isr_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
    }
    else
    {
        ISR_norm = 1;
        ISR_norm_up = 1;
    }
    //verification histogram
    if(conf->get("data_set") == "T5ZZ")
    {
        //cout<<"weight="<<weight<<", glu mass="<<phys.mass_gluino()<<endl;
	fill1DHistograms("susy_gluino_mass",phys.mass_gluino(),weight,allHistos,"",*n_gluino_bins,gluino_bins,rootdir);
    }

    if (conf->get("data_set") == "T5ZZ")
    {
            //isr_unc filled properly
        fill3DHistograms(prefix+"susy_type1MET_isr_up",g_met,phys.mass_gluino(),phys.mass_LSP(),(ISR_norm_up/ISR_norm)*(weight)*(1+(phys.isr_unc()/phys.isr_weight())),allSignal3DHistos, "(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with ISR SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);
    }
    else
    {
        //isr_unc is just deviation from not using the scale factor
        fill3DHistograms(prefix+"susy_type1MET_isr_up",g_met,phys.mass_gluino(),phys.mass_LSP(),(1/ISR_norm)*(weight)*(1/phys.isr_weight()),allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with ISR SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);
    }
   
    if(conf->get("data_set") != "TChiWZfullsim") 
    {
    	btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_norm->GetYaxis()->FindBin(phys.mass_LSP()));
    	btag_heavy_norm_up=1./g_btagsf_heavy_norm_up->GetBinContent(g_btagsf_heavy_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_heavy_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
    	btag_light_norm_up=1./g_btagsf_light_norm_up->GetBinContent(g_btagsf_light_norm_up->GetXaxis()->FindBin(phys.mass_gluino()), g_btagsf_light_norm_up->GetYaxis()->FindBin(phys.mass_LSP()));
    }
    else
    {
        btag_norm = 1.0;
        btag_heavy_norm_up = 1.0;
        btag_light_norm_up = 1.0;
    }

    fill3DHistograms(prefix+"susy_type1MET_btaglight_up",g_met,phys.mass_gluino(),phys.mass_LSP(),(btag_light_norm_up/btag_norm)*weight*(phys.weight_btagsf_light_UP()/phys.weight_btagsf()),allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with Light Btag SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);

    fill3DHistograms(prefix+"susy_type1MET_btagheavy_up",g_met,phys.mass_gluino(),phys.mass_LSP(),(btag_heavy_norm_up/btag_norm)*weight*(phys.weight_btagsf_heavy_UP()/phys.weight_btagsf()),allSignal3DHistos,"(x,y,z) = (met, m_glu, m_lsp). Type 1 MET with Heavy Btag SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_gluino_bins,gluino_bins,*n_lsp_bins,lsp_bins,rootdir);
}

void ZMETLooper::fillChiHists(std::string prefix)
{
    //Split the model from the signal region
    if (conf->get("data_set") == "TChiHZ_TChiZZ" || conf->get("data_set") == "TChiHZ")
    {
      n_chi_bins = &n_chi_bins_tchihz;
      chi_bins = chi_bins_tchihz;
    }

    else if (conf->get("data_set") == "TChiZZ")
    {
      n_chi_bins = &n_chi_bins_tchizz;
      chi_bins = chi_bins_tchizz;
    }

    if(prefix.find("SRHZ") != std::string::npos)
    {
        n_met_bins = &n_met_bins_tchizz_SRHZ;
        met_bins = met_bins_tchizz_SRHZ;
    }
    else if(prefix.find("SRVZResolved") != std::string::npos)
    {
        n_met_bins = &n_met_bins_tchizz_resolved;
        met_bins = met_bins_tchizz_resolved;
    }
    else if(prefix.find("SRVZBoosted") != std::string::npos)
    {
          n_met_bins = &n_met_bins_tchizz_boosted;
          met_bins = met_bins_tchizz_boosted;
    }
    else
    {
        n_met_bins = &n_met_bins_tchizz_resolved;
        met_bins = met_bins_tchizz_resolved;
    }


    fill2DHistograms(prefix+"susy_type1MET_counts",g_met,phys.mass_chi(),weight,allSignal2DHistos,"(x,y) = (met, m_chi). Type1MET for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);

    fill2DHistograms(prefix+"susy_type1MET_nowt",g_met,phys.mass_chi(),1.0,allSignal2DHistos,"(x,y) = (met, m_chi). Type1MET with no event weights for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);

    ISR_norm=1./g_isr_norm->GetBinContent(g_isr_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);
    btag_norm=1./g_btagsf_norm->GetBinContent(g_btagsf_norm->GetXaxis()->FindBin(phys.mass_chi()), 1);

    ISR_norm_up=1./g_isr_norm_up->GetBinContent(g_isr_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);

    btag_heavy_norm_up=1./g_btagsf_heavy_norm_up->GetBinContent(g_btagsf_heavy_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);
    btag_light_norm_up=1./g_btagsf_light_norm_up->GetBinContent(g_btagsf_light_norm_up->GetXaxis()->FindBin(phys.mass_chi()), 1);

    fill2DHistograms(prefix+"susy_type1MET_btagheavy_up",g_met,phys.mass_chi(),(btag_heavy_norm_up/btag_norm)*weight*(phys.weight_btagsf_heavy_UP()/phys.weight_btagsf()),allSignal2DHistos,"(x,y) = (met, m_chi). Type 1 MET with Heavy Btag SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);

    fill2DHistograms(prefix+"susy_type1MET_btaglight_up",g_met,phys.mass_chi(),(btag_light_norm_up/btag_norm)*weight*(phys.weight_btagsf_light_UP()/phys.weight_btagsf()),allSignal2DHistos,"(x,y) = (met, m_chi). Type 1 MET with Light Btag SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);

    fill2DHistograms(prefix+"susy_type1MET_isr_up",g_met,phys.mass_chi(),(1/ISR_norm)*(weight)*(1/phys.isr_weight()),allSignal2DHistos,"(x,y) = (met, m_chi). Type 1 MET with ISR SF fluctuated up for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);

    double tau21_weight_up,tau21_weight_down;
    if(prefix.find("SRVZBoosted") != std::string::npos)
    {
        tau21_weight_up = weight * fatJetScaleFactor(1);
        tau21_weight_down = weight * fatJetScaleFactor(-1);

        fill2DHistograms(prefix+"susy_type1MET_tau21_up",g_met,phys.mass_chi(),tau21_weight_up,allSignal2DHistos,"(x,y) = (met, m_chi). Type1MET with tau21 up weights for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);
        
        fill2DHistograms(prefix+"susy_type1MET_tau21_down",g_met,phys.mass_chi(),tau21_weight_down,allSignal2DHistos,"(x,y) = (met, m_chi). Type1MET with tau21 down weights for"+g_sample_name,*n_met_bins,met_bins,*n_chi_bins,chi_bins,rootdir);


    }

}

void ZMETLooper::fillFSMETHists(std::string prefix)
{
    /*Special function to fill Rsfof multiplied flavour symmetric histograms
     * Fills 7 histograms, one for central value, and one for each systematic
     * uncertainty. Calls the Rsfof computing function and multiplies the weight
     * with the already calculated weight from other contributions, to do this
     * properly*/

    double weight_FS = weight * computeRsfof();
    double weight_FS_norm_up = weight * computeRsfof(1);
    double weight_FS_norm_down = weight * computeRsfof(-1);
    double weight_FS_pt_up = weight * computeRsfof(2);
    double weight_FS_pt_down = weight * computeRsfof(-2);
    double weight_FS_eta_up = weight * computeRsfof(3);
    double weight_FS_eta_down = weight * computeRsfof(-3);

    //Fill the FS histograms
    fill1DHistograms(prefix+"FS_type1MET",g_met,weight_FS,allHistos,"",6000,0,6000,rootdir);

    fill1DHistograms(prefix+"FS_type1MET_norm_up",g_met,weight_FS_norm_up,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"FS_type1MET_norm_down",g_met,weight_FS_norm_down,allHistos,"",6000,0,6000,rootdir);

    fill1DHistograms(prefix+"FS_type1MET_pt_up",g_met,weight_FS_pt_up,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"FS_type1MET_pt_down",g_met,weight_FS_pt_down,allHistos,"",6000,0,6000,rootdir);

    fill1DHistograms(prefix+"FS_type1MET_eta_up",g_met,weight_FS_eta_up,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"FS_type1MET_eta_down",g_met,weight_FS_eta_down,allHistos,"",6000,0,6000,rootdir);

}

void ZMETLooper::fillallHistograms(std::string prefix)
{
    std::string SR;
    std::string commonHistPrefix = prefix;
    //  double weight;
    if(conf->get("signal_region") == "all")
    {
      SR = prefix;
      weight = getWeight(TString(SR.c_str()));
    }
    else
    {
      weight = getWeight();
    }
    if(conf->get("dil_flavor") == "all")
    {
            if(dil_flavor == 0)
                commonHistPrefix += "ee_";
            else if(dil_flavor == 1)
                commonHistPrefix += "mumu_";
            else if(dil_flavor == 2)
                commonHistPrefix += "emu_";
    }
    //VERY POORLY WRITTEN CODE COMIN' UP
    if(conf->get("FS_mass_window_study") == "true")
    {
      if(dil_flavor == 2)
        fillMassWindowHistograms(prefix);
    }
    else
    {
        //print out stuff
        fillCommonHists(commonHistPrefix);

        //Rsfof new stuff
        if(dil_flavor == 2 && conf->get("R_sfof_from_json") == "true" && conf->get("signal_region") == "all")

        {
            //Rsfof multiplied emu histograms, and appropriate error histograms
            fillFSMETHists(commonHistPrefix);
        }

        sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();

        if(conf->get("event_type") == "photon")
            fillPhotonCRHists(prefix);
        if(conf->get("TemplatesClosure") == "true")
        {
          fillClosureHists(prefix);
        }

        //===========================================
        // Signal Region Specific Histos
        //===========================================

        if(SR.find("SRVZ") != std::string::npos or conf->get("VZ_hists") == "true")
        {
            fillTChiWZHists(prefix);
        }
        if(SR == "SRHZ" || conf->get("HZ_hists") == "true")
        {
            fillTChiHZHists(prefix);
        }

        if(SR.find("Boosted") != std::string::npos)
        {
            if(SR.find("SR") != std::string::npos)
            {
                fillBoostedHists(g_fatjet_indices,commonHistPrefix);
            }
            else if(SR.find("VR") != std::string::npos)
            {
                fillBoostedHists(g_fatjet_validation_indices,commonHistPrefix);
            }
            else
            {
                fillBoostedHists(g_fatjet_inclusive_indices,commonHistPrefix);
            }
        }

        else if(conf->get("boosted_histograms") == "true")
        {
            fillBoostedHists(g_fatjet_indices,commonHistPrefix); 
        }

        if (conf->get("GammaMuStudy") == "true")
        {
            fillGammaMuCRHists(prefix);
        }
        if(conf->get("dilep_control_region") == "true")
        {
            std::string dilepPrefix = prefix;
            if(dil_flavor == 0)
                dilepPrefix += "ee_";
            else if(dil_flavor == 1)
                dilepPrefix += "mumu_";
            else if(dil_flavor == 2)
                dilepPrefix += "emu_";

            fillDileptonCRHists(dilepPrefix);
        }

        if(conf->get("SUSY_Glu_LSP_scan") == "true")
        {
            fillGluLSPHists(commonHistPrefix);
        }
        else if(conf->get("SUSY_chi_scan") == "true")
        {
            fillChiHists(commonHistPrefix);
        }

        if(conf->get("ECalTest") != "")
        {
            fillEcalHists();
        }
    }
}


bool ZMETLooper::passStrongSRCuts()
{
    if(passSRACuts())
    {
       if(printFail) cout<<phys.evt()<<" Passed SRA"<<endl;
       commonHistPrefix = "SRA";
    }
    else if(passSRAbCuts())
    {
      if(printFail) cout<<phys.evt()<<" Passed SRAb"<<endl;
      commonHistPrefix = "SRAb";
    }
    else if(passSRBCuts())
    {
      if(printFail) cout<<phys.evt()<<" Passed SRB"<<endl;
      commonHistPrefix = "SRB";
    }
    else if(passSRBbCuts())
    {
      if(printFail) cout<<phys.evt()<<" Passed SRBb"<<endl;
      commonHistPrefix = "SRBb";
    }
    else if(passSRCCuts())
    {
      if(printFail) cout<<phys.evt()<<" Passed SRC"<<endl;
      commonHistPrefix = "SRC";
    }
    else if(passSRCbCuts())
    {
      if(printFail) cout<<phys.evt()<<" Passed SRCb"<<endl;
      commonHistPrefix = "SRCb";
    }
    else
    {
      return false;
    }
    fillallHistograms(commonHistPrefix);
    return true;
}

bool ZMETLooper::passStrongVRCuts()
{
    if(passVRACuts())
    {
        commonHistPrefix = "VRA";
        if(printFail) cout<<"Passed VRA"<<endl;
    }
    else if(passVRBCuts())
    {
        commonHistPrefix = "VRB";
        if(printFail) cout<<"Passed VRB"<<endl;
    }
    else if(passVRCCuts())
    {
        commonHistPrefix = "VRC";
        if(printFail) cout<<"Passed VRC"<<endl;
    }
    else
    {
        return false;
    }
    fillallHistograms(commonHistPrefix);
    return true;
}

bool ZMETLooper::passEWKSRCuts()
{
  /*Implementing Strategy A for SRWZ - veto events with fat jet in resolved region*/
  bool flag=false;
  if(passSRVZCuts())
  {
    commonHistPrefix = "SRVZResolved";
    if(printFail) cout<<"Passed SRVZ Resolved"<<endl;
    fillallHistograms(commonHistPrefix);
    flag = true;
  }

  if(conf->get("2016_reproduce") != "true"  && passSRVZBoostedCuts())
  {
    commonHistPrefix = "SRVZBoosted";
    if(printFail) cout<<"Passed SRVZ Boosted"<<endl;
    fillallHistograms(commonHistPrefix);
    flag = true;
  }

  if(conf->get("2016_reproduce") != "true" && passSRVZBoostedCuts(-1) && !phys.isData())
  {
      commonHistPrefix = "SRVZBoosted_JMSDown";
      fillallHistograms(commonHistPrefix);
      flag = true;
  }

  if(conf->get("2016_reproduce") != "true" && passSRVZBoostedCuts(1) && !phys.isData())
  {
      commonHistPrefix = "SRVZBoosted_JMSUp";
      fillallHistograms(commonHistPrefix);
      flag = true;
  }



  if(conf->get("2016_reproduce") != "true" && passSRVZBoostedCuts(0,1) && !phys.isData())
  {
      commonHistPrefix = "SRVZBoosted_JMRUp";
      fillallHistograms(commonHistPrefix);
      flag = true;
  }

  if(conf->get("2016_reproduce") != "true" && passSRVZBoostedCuts(0,-1) && !phys.isData())
  {
      commonHistPrefix = "SRVZBoosted_JMRDown";
      fillallHistograms(commonHistPrefix);
      flag = true;
  }



  if(passSRHZCuts())
  {
      commonHistPrefix = "SRHZ";
      if(printFail) cout<<"Passed SRHZ"<<endl;
      flag = true;
      fillallHistograms(commonHistPrefix);
  }

  return flag;
}

bool ZMETLooper::passEWKVRCuts()
{
    bool flag = false;

    if(passVRWZCuts())
    {
        commonHistPrefix = "VRWZResolved";
        if(printFail) cout<<"Passed VRWZ Resolved"<<endl;
        flag = true;
    }

    if(conf->get("2016_reproduce") != "true" && passVRWZBoostedCuts())
    {
        commonHistPrefix = "VRWZBoosted";
        if(printFail) cout<<"Passed VRWZ Boosted"<<endl;
        flag = true;
    }
    if(flag == true)
    {
        fillallHistograms(commonHistPrefix);
    }

    if(passVRHZCuts())
    {
        commonHistPrefix = "VRHZ";
        if(printFail) cout<<"Passed VRHZ"<<endl;
        flag = true;
        fillallHistograms(commonHistPrefix);
    }
    return flag;
}


bool ZMETLooper::passInclusiveCuts()
{
  return true; //Temporary
}

void ZMETLooper::fillCommonHists(std::string prefix)
{
    //create histograms
    fill1DHistograms(prefix+"weight_log",log10(abs(weight)),1,allHistos,"",n_weight_log_bins,weight_log_bins,rootdir);
    fill1DHistograms(prefix+"weight_log_flat",abs(weight),1,allHistos,"",101,0,1.01,rootdir);
    fill1DHistograms(prefix+"numMETFilters",sumMETFilters,1,allHistos,"",50,0,50,rootdir);

    if(g_met != 0)
    {
        fill1DHistograms(prefix+"type1MET",g_met,weight,allHistos,"",6000,0,6000,rootdir);
        if(prefix.find("SRC") != std::string::npos)
        {
            fill1DHistograms(prefix+"type1MET_widebin",g_met,weight,allHistos,"",n_met_bins_t5zznat_SRC,met_bins_t5zznat_SRC,rootdir);
        }
        else if(prefix.find("SRHZ") != std::string::npos)
        {
            fill1DHistograms(prefix+"type1MET_widebin",g_met,weight,allHistos,"",n_met_bins_tchihz,met_bins_tchihz,rootdir);
        }
        else if(prefix.find("Resolved") != std::string::npos)
        {
            fill1DHistograms(prefix+"type1MET_widebin",g_met,weight,allHistos,"",n_met_bins_tchiwz_resolved,met_bins_tchiwz_resolved,rootdir);
        }
        else if(prefix.find("Boosted") != std::string::npos)
        {
            fill1DHistograms(prefix+"type1MET_widebin",g_met,weight,allHistos,"",n_met_bins_tchiwz_boosted,met_bins_tchiwz_boosted,rootdir);
        }
        else
        {
            fill1DHistograms(prefix+"type1MET_widebin",g_met,weight,allHistos,"",n_met_bins_t5zznat,met_bins_t5zznat,rootdir); 
        }
    }

    if (phys.met_rawPt() != 0) //rawmet->Fill(phys.met_rawPt(), weight);
    {
        fill1DHistograms(prefix+"rawMET",phys.met_rawPt(),weight,allHistos,"",6000,0,6000,rootdir);
    }
    if (g_ht != 0) 
    {
        fill1DHistograms(prefix+"ht",g_ht,weight,allHistos,"",6000,0,6000,rootdir);
        fill1DHistograms(prefix+"ht_wide",g_ht,weight,allHistos,"",60,0,6000,rootdir);
    }

    if(g_mht != 0)
    {
        
        fill1DHistograms(prefix+"mht",g_mht,weight,allHistos,"",6000,0,6000,rootdir);
        TVector2 mhtVector = TVector2(g_mht * cos(g_mht_phi),g_mht * sin(g_mht_phi));
        TVector2 metVector = TVector2(g_met * cos(g_met_phi),g_met*sin(g_met_phi));
        mhtMETDifference = (mhtVector - metVector).Mod();
        fill1DHistograms(prefix+"mhtDiffBymet",mhtMETDifference/g_met,weight,allHistos,"",1000,0,100,rootdir);
    }
    if (phys.gen_ht() != 0)
    {
        fill1DHistograms(prefix+"genht",phys.gen_ht(),weight,allHistos,"",6000,0,6000,rootdir);
    }
    if(!phys.isData() && conf->get("event_type") == "photon" && phys.gamma_genPt().at(0) >= 0)
    {
        fill1DHistograms(prefix+"gamma_genpt",phys.gamma_genPt().at(0),weight,allHistos,"",6000,0,6000,rootdir);
    }
    if (bosonPt() != 0)
    {
        fill1DHistograms(prefix+"vpt",bosonPt(),weight,allHistos,"",n_ptbins_std,ptbins_std,rootdir);
        fill1DHistograms(prefix+"vpt_fine",bosonPt(),weight,allHistos,"",n_ptbins_fine,ptbins_fine,rootdir);
        fill1DHistograms(prefix+"vpt_flat",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);
    }

    fill1DHistograms(prefix+"njets",g_njets,weight,allHistos,"",50,0,50,rootdir);
    fill1DHistograms(prefix+"nbtags_m",g_nBJetMedium25,weight,allHistos,"",50,0,50,rootdir);
    fill1DHistograms(prefix+"nbtags_l",g_nBJetLoose25,weight,allHistos,"",50,0,50,rootdir);

    fill1DHistograms(prefix+"nbtags_t",phys.nBJetTight25(),weight,allHistos,"",50,0,50,rootdir);

    fill1DHistograms(prefix+"nVert",phys.nVert(),weight,allHistos,"",150,0,150,rootdir);
    fill1DHistograms(prefix+"nlep",phys.nlep(),weight,allHistos,"",20,0,20,rootdir);
    fill1DHistograms(prefix+"nisotrack",phys.nisoTrack_mt2(),weight,allHistos,"",20,0,20,rootdir);
    
    fill1DHistograms(prefix+"dRll",phys.dRll(),weight,allHistos,"",1000,0,10,rootdir);
    if (g_mt2 != 0 )
    {
        fill1DHistograms(prefix+"mt2",g_mt2,weight,allHistos,"",1000,0,1000,rootdir);
    }
    if (g_mt2b != 0 )
    {
        fill1DHistograms(prefix+"mt2b",g_mt2b,weight,allHistos,"",6000,0,6000,rootdir);
    }
    //cout<<__LINE__<<endl;
    if (g_njets > 0)
    {
        fill1DHistograms(prefix+"dphi_jet1_met",acos(cos(g_met_phi - g_jets_p4.at(0).phi())),weight,allHistos,"",100,0,3.15,rootdir);
    }
    //cout<<__LINE__<<endl;
    if (g_njets > 1)
    {
        fill1DHistograms(prefix+"dphi_jet2_met",acos(cos(g_met_phi - g_jets_p4.at(1).phi())),weight,allHistos,"",100,0,3.15,rootdir);
    }
    if(g_njets > 2)
    {
        fill1DHistograms(prefix+"dphi_jet3_met",acos(cos(g_met_phi - g_jets_p4.at(2).phi())),weight,allHistos,"",100,0,3.15,rootdir);
    }

   if(conf->get("photon_pt_test") == "true")
    {
        fill2DHistograms(prefix+"PtvEta",phys.gamma_p4().at(0).pt(),phys.gamma_p4().at(0).eta(),weight,all2DHistos,"",1000,0,1000,100,-2.4,2.4,rootdir);
        fill2DHistograms(prefix+"PtvPhi",phys.gamma_p4().at(0).pt(),phys.gamma_p4().at(0).phi(),weight,all2DHistos,"",1000,0,1000,200,-6.28,6.28,rootdir);

    }
}

void ZMETLooper::fillMassWindowHistograms(std::string prefix)
{
    //Fill counts (Mll > 20) and counts (86 < Mll < 96) here
    if(phys.dilmass() > 20)
    {
      fill1DHistograms(prefix+"count_wideband",1,weight,allHistos,"",2,0,2,rootdir);
    }
    if(phys.dilmass() > 86 && phys.dilmass() < 96)
    {
      fill1DHistograms(prefix+"count_narrowband",1,weight,allHistos,"",2,0,2,rootdir);
    }

    //temp stuff
    fill1DHistograms(prefix+"dilmass",phys.dilmass(),weight,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"mt2",g_mt2,weight,allHistos,"",6000,0,6000,rootdir);

}

void ZMETLooper::fillBoostedHists(std::vector<size_t> g_fatjet_indices,std::string prefix)
{

    //tau21 up and down variation MET histogram
    double tau21_weight_up, tau21_weight_down;
    double tau21_central = fatJetScaleFactor(0);
    double tau21_unc;
    tau21_unc = fatJetScaleFactor(1);
    for(int iJet = 0; iJet < phys.ak8jets_p4().size(); iJet++)
    {
        tau21_unc = sqrt(tau21_unc * tau21_unc + fatJetPtError(phys.ak8jets_p4().at(iJet).pt(),g_year,tau21) * fatJetPtError(phys.ak8jets_p4().at(iJet).pt(),g_year,tau21));
    }

    tau21_weight_up = weight * (1 + tau21_unc/tau21_central);
    tau21_weight_down = weight * (1 - tau21_unc/tau21_central);

 
        fill1DHistograms(prefix+"type1MET_tau21_up",g_met,tau21_weight_up,allHistos,"",6000,0,6000,rootdir);
        fill1DHistograms(prefix+"type1MET_tau21_down",g_met,tau21_weight_down,allHistos,"",6000,0,6000,rootdir);

  

    fill1DHistograms(prefix+"nFatJets",phys.nFatJets(),weight,allHistos,"",50,0,50,rootdir);

    fill1DHistograms(prefix+"dphi_met_fatjet",g_dphi_met_fatjet,weight,allHistos,"",100,0,3.14,rootdir);

    for(auto &iJet:g_fatjet_indices)
    {
        if(phys.ak8jets_tau2().at(iJet) != 0 && phys.ak8jets_tau1().at(iJet) != 0)
        {
            fill1DHistograms(prefix+"tau21",phys.ak8jets_tau2().at(iJet)/phys.ak8jets_tau1().at(iJet),weight,allHistos,"",1000,0,10,rootdir);

        }
        fill1DHistograms(prefix+"softDropMass",phys.ak8jets_softDropMass().at(iJet),weight,allHistos,"",6000,0,6000,rootdir);
        //fill1DHistograms(prefix+"softDropMass_old",phys.ak8jets_original_softDropMass().at(iJet),weight,allHistos,"",6000,0,6000,rootdir); 
        fill1DHistograms(prefix+"fat_jet_pt",phys.ak8jets_p4().at(iJet).pt(),weight,allHistos,"",6000,0,6000,rootdir);
        fill1DHistograms(prefix+"fat_jet_eta",phys.ak8jets_p4().at(iJet).eta(),weight,allHistos,"",200,-3,3,rootdir);
        fill1DHistograms(prefix+"fat_jet_phi",phys.ak8jets_p4().at(iJet).phi(),weight,allHistos,"",200,-6.28,6.28,rootdir);
    }
}

void ZMETLooper::fillPhotonCRHists(std::string prefix)
{

    fill1DHistograms(prefix+"photonPt",phys.gamma_p4().at(0).pt(),weight,allHistos,"",1000,0,1000,rootdir);
    fill1DHistograms(prefix+"photonEta",phys.gamma_p4().at(0).eta(),weight,allHistos,"",200,-2.4,2.4,rootdir);
    fill1DHistograms(prefix+"photonPhi",phys.gamma_p4().at(0).phi(),weight,allHistos,"",400,-6.30,6.30,rootdir);
}

void ZMETLooper::fillGammaMuCRHists(std::string prefix)
{
    fill1DHistograms(prefix+"MT_MuMET",getMTLepMET(),weight,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"dR_GammaMu",getdRGammaLep(),weight,allHistos,"",200,0,5.8,rootdir);
    fill1DHistograms(prefix+"mu_pt",phys.lep_pt().at(0),weight,allHistos,"",6000,0,6000,rootdir);
}

void ZMETLooper::fillDileptonCRHists(std::string prefix)
{

    fill1DHistograms(prefix+"dilmass",phys.dilmass(),weight,allHistos,"",6000,0,6000,rootdir);
    fill1DHistograms(prefix+"ll_pt",phys.lep_pt().at(0),weight,allHistos,"",1000,0,1000,rootdir);
    fill1DHistograms(prefix+"lt_pt",phys.lep_pt().at(1),weight,allHistos,"",1000,0,1000,rootdir);
    fill1DHistograms(prefix+"ll_eta",phys.lep_p4().at(0).eta(),weight,allHistos,"",200,-2.4,2.4,rootdir);
    fill1DHistograms(prefix+"lt_eta",phys.lep_p4().at(1).eta(),weight,allHistos,"",200,-2.4,2.4,rootdir);
    fill1DHistograms(prefix+"ll_phi",phys.lep_p4().at(0).phi(),weight,allHistos,"",200,-6.28,6.28,rootdir);
    fill1DHistograms(prefix+"lt_phi",phys.lep_p4().at(1).phi(),weight,allHistos,"",200,-6.28,6.28,rootdir);

}


void ZMETLooper::fillClosureHists(std::string prefix)
{
    fill1DHistograms(prefix+"genRecoHT",abs(phys.gen_ht() - g_ht),weight,allHistos,"",1000,0,1000,rootdir);
    fill1DHistograms(prefix+"evtscale1fb",phys.evt_scale1fb(),weight,allHistos,"",1000,0,300,rootdir);
    fill2DHistograms(prefix+"PtvMET",g_met,bosonPt(),weight,all2DHistos,"",6000,0,6000,n_ptbins_std,ptbins_std,rootdir);
    fill2DHistograms(prefix+"PtFinevsMET",g_met,bosonPt(),weight,all2DHistos,"",6000,0,6000,n_ptbins_fine,ptbins_fine,rootdir);
}

void ZMETLooper::fillEcalHists(std::string prefix)
{
    fill1DHistograms(prefix+"dphi_gamma_met",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
    if(g_met >= 100)
    {
        fill1DHistograms(prefix+"dphi_gamma_met100",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
        fill1DHistograms(prefix+"pt_gamma_met100",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);

    }

    if(g_met >= 200)
    {
        fill1DHistograms(prefix+"dphi_gamma_met200",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
        fill1DHistograms(prefix+"pt_gamma_met200",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);

    }

    if(g_met >= 300)
    {
        fill1DHistograms(prefix+"dphi_gamma_met300",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
        fill1DHistograms(prefix+"pt_gamma_met300",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);

    }

    if(g_met >= 400)
    {
        fill1DHistograms(prefix+"dphi_gamma_met400",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
        fill1DHistograms(prefix+"pt_gamma_met400",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);

    }

    if(g_met >= 500)
    {
        fill1DHistograms(prefix+"dphi_gamma_met500",dphi_gm,weight,allHistos,"",100,0,3.15,rootdir);
        fill1DHistograms(prefix+"pt_gamma_met500",bosonPt(),weight,allHistos,"",6000,0,6000,rootdir);

    }
    float dphi_jet1_gamma = acos(cos(g_met_phi - phys.jets_p4().at(0).phi()));
    float dphi_jet2_gamma = acos(cos(g_met_phi - phys.jets_p4().at(1).phi()));
    fill1DHistograms(prefix+"dphi_gamma_jet1",dphi_jet1_gamma,weight,allHistos,"",100,0,3.15,rootdir);
    fill1DHistograms(prefix+"dphi_gamma_jet1",dphi_jet2_gamma,weight,allHistos,"",100,0,3.15,rootdir);

}

void ZMETLooper::fillTChiWZHists(std::string prefix)
{
    fill1DHistograms(prefix+"mjj_min_dphi",g_mjj_mindphi,weight,allHistos,"",6000,0,6000,rootdir);
}

void ZMETLooper::fillTChiHZHists(std::string prefix)
{
    fill1DHistograms(prefix+"sum_mlb",phys.sum_mlb(),weight,allHistos,"",6000,0,6000,rootdir);
    if(conf->get("NBjets_loose_min") != "")
    {
        fill1DHistograms(prefix+"m_bb_csv",getMbb(),weight,allHistos,"",6000,0,6000,rootdir);
        fill1DHistograms(prefix+"m_bb_bpt",getMbb(),weight,allHistos,"",6000,0,6000,rootdir);
        mt2_val_fromb = getMT2ForBjets(true);
        if(mt2_val_fromb != 0)
        {
            fill1DHistograms(prefix+"mt2_fromb",mt2_val_fromb,weight,allHistos,"",6000,0,6000,rootdir);
        }
    }
    else
    {

        fill1DHistograms(prefix+"m_bb_csv",g_mbb,weight,allHistos,"",6000,0,6000,rootdir);
        fill1DHistograms(prefix+"m_bb_bpt",phys.mbb_bpt(),weight,allHistos,"",6000,0,6000,rootdir);

        mt2_val_fromb = getMT2ForBjets();
        if(mt2_val_fromb != 0)
        {
            fill1DHistograms(prefix+"mt2_fromb",mt2_val_fromb,weight,allHistos,"",6000,0,6000,rootdir);
        }

    }

    if(phys.mt2j()!=0)
    {
        fill1DHistograms(prefix+"mt2j",phys.mt2j(),weight,allHistos,"",6000,0,6000,rootdir);
    }

    if (phys.nlep() > 1 && g_nBJetMedium35 >= 2)
    {
        mt2_val_hz = getMT2HiggsZ();
    }
        if (mt2_val_hz != 0)
        {
            fill1DHistograms(prefix+"mt2_hz",mt2_val_hz,weight,allHistos,"",6000,0,6000,rootdir);
        }

        //cout<<__LINE__<<endl;

        if (g_mt2 != 0 && g_mt2b != 0 )
        {
            fill2DHistograms(prefix+"MT2_MT2B",g_mt2,g_mt2b,weight,all2DHistos,"",6000,0,6000,6000,0,6000,rootdir);
        }


        if (g_mt2 != 0 && mt2_val_fromb != 0 )
        {
            fill2DHistograms(prefix+"MT2_MT2_fromb",g_mt2,mt2_val_fromb,weight,all2DHistos,"",6000,0,6000,6000,0,6000,rootdir);
        }


        if (g_mt2 != 0 && mt2_val_hz != 0 )
        {
            fill2DHistograms(prefix+"MT2_MT2_HZ",g_mt2,mt2_val_hz,weight,all2DHistos,"",6000,0,6000,6000,0,6000,rootdir);
        }

        //cout<<__LINE__<<endl;

        if (g_nBJetMedium35 >= 2)
        {
            pair<int,int> b_index = getMostBlike();
            bb_pt = (g_jets_p4.at(b_index.first) + g_jets_p4.at(b_index.second)).pt();
            fill1DHistograms(prefix+"sum_pt_z_bb",bb_pt+phys.dilpt(),weight,allHistos,"",6000,0,6000,rootdir);
        }
}

int ZMETLooper::compute_isotrack_mt2()
{
    bool signalLeptonOverlapFlag;
    int nisoTrack_mt2 = 0;
    for(auto &pit:phys.isotrack_p4())
    {
        signalLeptonOverlapFlag  = false;
        for(auto &ilep:phys.lep_p4())
        {
            if(DeltaR(pit,ilep) < 0.01)
            {
                signalLeptonOverlapFlag = true;
                break;
            }
        }
        if(!signalLeptonOverlapFlag)
            nisoTrack_mt2++;
    }
    return nisoTrack_mt2;
}

bool ZMETLooper::passHEM1516Veto()
{
    //returns false if objects found in veto regions
    float eta_low = -4.7;
    float eta_high = -1.4;
    float phi_low = -1.6;
    float phi_high = -0.8;
    //tight electrons
    for(size_t i = 0; i < phys.loose_lep_p4().size(); i++)
    {
        if(abs(phys.loose_lep_pdgid().at(i)) == 11)
        {
            if((phys.loose_lep_p4().at(i).eta() > eta_low && phys.loose_lep_p4().at(i).eta() < eta_high) && (phys.loose_lep_p4().at(i).phi() > phi_low && phys.loose_lep_p4().at(i).phi() < phi_high))
            {
                return false;
            }
        }
    }

    //jets
    for(size_t i = 0; i < phys.jets_p4_wide_eta().size(); i++)
    {
            if((phys.jets_p4_wide_eta().at(i).eta() > eta_low && phys.jets_p4_wide_eta().at(i).eta() < eta_high) && (phys.jets_p4_wide_eta().at(i).phi() > phi_low && phys.jets_p4_wide_eta().at(i).phi() < phi_high))
            {
                return false;
            }

    }

    //fatjets
    for(size_t i = 0; i < phys.ak8jets_p4_wide_eta().size(); i++)
    {
        if((phys.ak8jets_p4_wide_eta().at(i).eta() > eta_low && phys.ak8jets_p4_wide_eta().at(i).eta() < eta_high) && (phys.ak8jets_p4_wide_eta().at(i).phi() > phi_low && phys.ak8jets_p4_wide_eta().at(i).phi() < phi_high))
            {
                return false;
            }
    }

    //isotracks
    for(size_t i = 0; i<phys.isotrack_p4().size(); i++)
    {
            if((phys.isotrack_p4().at(i).eta() > eta_low && phys.isotrack_p4().at(i).eta() < eta_high) && (phys.isotrack_p4().at(i).phi() > phi_low && phys.isotrack_p4().at(i).phi() < phi_high))
            {
                return false;
            }

    }

    return true;
} 


//Taken from here : https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging#Recommendations_for_13_TeV_data
//
//one for every fat jet found!
double ZMETLooper::tau21WP()
{
    if(g_year == 2016)
    {
        return 0.4;
    }
    else if(g_year == 2017)
    {
        return 0.45;
    }
    else if(g_year == 2018)
    {
        return 0.45;
    }
    else return 0.6;
}


double ZMETLooper::fatJetJMSScaleFactor(int mode)
{
    double central_value,uncertainty;
    if(g_year == 2016)
    {
        central_value = 1;
        uncertainty = 0.0094;
    }
    else if(g_year == 2017)
    {
        central_value = 0.982;
        uncertainty = 0.004;
    }
    else if(g_year == 2018)
    {
        central_value = 0.997;
        uncertainty = 0.004;
    }
    else
    {
        cout<<"Wrong year!"<<endl;
        return 1.0;
    }

    //Fancy shit because I want to show off
    //mode : 0->central, 1 -> vary up, -1 -> vary down
    //
    if(!phys.isData())
    {
        if(mode == 1)
        {
            return 1 + uncertainty/central_value;        
        }
        else if(mode == -1)
        {
            return 1 - uncertainty/central_value;
        }
        else return central_value;
    }
    else
    {
        return 1.0;
    }
}

double ZMETLooper::fatJetScaleFactor(int mode)
{
    double central_value, uncertainty;

    if(g_year == 2016)
    {
        central_value = 1.00;
        uncertainty = 0.06;
    }
    else if(g_year == 2017)
    {
        central_value = 0.97;
        uncertainty = 0.06;
    }
    else if(g_year == 2018)
    {

        central_value = 0.980;
        uncertainty = 0.027;
    }
    else
    {
        cout<<"Wrong year!"<<endl;
        return 1.0;
    }

    //Fancy shit because I want to show off
    //mode : 0->central, 1 -> vary up, -1 -> vary down
    //
    if(!phys.isData())
    {
        if(mode == 1)
        {
            return  uncertainty;
        }
        else if(mode == -1)
        {
            return uncertainty;
        }
        else return central_value;
    }
    else
    {
        return 1.0;
    }
}


double ZMETLooper::fatJetPtError(float pt, int year, double tau21)
{
    if(phys.isData())
    {
        return 0.0;
    }

    if(year == 2017)
    {
        if(pt > 200 && pt < 250)
        {
            return 0.07;
        }
        else if(pt > 250 && pt < 300)
        {
            return 0.06;
        }
        else if(pt > 300 && pt < 350)
        {
            return 0.09;
        }
        else if(pt > 350 && pt < 600)
        {
            return 0.13;
        }
        else if(pt > 600)
        {
            return 0.085 * tau21 * log(pt/200);
        }

        else return 0.0;
    }
    else 
    {
        return 0.085 * tau21 * log(pt/200);
    }
}
