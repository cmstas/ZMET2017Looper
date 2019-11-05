#include "ScanChain.h"


ZMETLooper::ZMETLooper(int year)
{
    nDuplicates = 0;
    year_fromCommandLine = year;
    num_events_veto_ttbar = 0;
    num_events_veto_ttgamma = 0;
    MCTriggerEmulation = true;

    currentFile = nullptr;
    printStats = false;
    printFail = false;

}

int ZMETLooper::getYear()
{
    return year_fromCommandLine;
}


void ZMETLooper::initSyncFiles(TString savePath)
{
    syncFile_baseline = new fstream;
    syncFile_emu = new fstream;
    syncFile_njets = new fstream;
    syncFile_metFilter = new fstream;
    syncFile_met = new fstream;
    syncFile_isotracks = new fstream;
    std::string file_prefix = std::string(savePath.Data()) + conf->get("Name");

    syncFile_baseline->open(file_prefix + "_baseline.txt",std::ios::out);
    syncFile_emu->open(file_prefix +"_emu.txt",std::ios::out);
    syncFile_njets->open(file_prefix+"_njets.txt",std::ios::out);
    syncFile_metFilter->open(file_prefix+"_metFilter.txt",std::ios::out);
    syncFile_met->open(file_prefix+"_met.txt",std::ios::out);
    syncFile_isotracks->open(file_prefix+"_isotracks.txt",std::ios::out);

    *syncFile_baseline<<"Run,Lumi,Event,PD"<<std::endl;
    *syncFile_emu<<"Run,Lumi,Event,PD"<<std::endl;
    *syncFile_njets<<"Run,Lumi,Event,PD"<<std::endl;
    *syncFile_metFilter<<"Run,Lumi,Event,PD"<<std::endl;
    *syncFile_met<<"Run,Lumi,Event,PD"<<std::endl;
    *syncFile_isotracks<<"Run,Lumi,Event,PD"<<std::endl;
}

void ZMETLooper::writeSyncFile(fstream* syncFile)
{
    *syncFile<<phys.run()<<","<<phys.lumi()<<","<<phys.evt()<<","<<phys.evt_dataset().at(0)<<endl;
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




double ZMETLooper::DeltaR(const LorentzVector p1, const LorentzVector p2){
  /*Returns the DeltaR between objects p1 and p2.*/
  //cout<<__LINE__<<endl;
  return sqrt( (p1.eta() - p2.eta())*(p1.eta() - p2.eta())+(p1.phi() - p2.phi())*(p1.phi() - p2.phi()) );
}

TFile VetoFile("External/veto_etaphi.root");


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
            if(phys.HLT_Photon200() > 0 && phys.gamma_pt().at(0) > 210 && g_year != 2016) return true; //need to do trigger efficiency tests
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
//    else if( !phys.HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 40 && phys.gamma_pt().at(0) < 55    ) return true;
//    else if( !phys.HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 && phys.gamma_pt().at(0) < 40    ) return true;
//    else if( !phys.HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ) return true;
            return false;
    }
}

bool ZMETLooper::passMuonTriggers(){
  if ( (! MCTriggerEmulation) && (! phys.isData()) ){
    return true;
  }
  else{
    //cout<<__LINE__<<endl;
      //cout<<"Using Non DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      //if (printStats) { cout<<"HLT_DoubleMu_nonDZ: "<<phys.HLT_DoubleMu_nonDZ()<<" HLT_DoubleMu_tk_nonDZ: "<<phys.HLT_DoubleMu_tk_nonDZ()<<" "<<" HLT_DoubleMu_noiso: "<<phys.HLT_DoubleMu_noiso()<<" "; }
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
    //if (printStats) { cout<<"HLT_DoubleEl_DZ_2: "<<phys.HLT_DoubleEl_DZ_2()<<" HLT_DoubleEl_noiso: "<<phys.HLT_DoubleEl_noiso()<<" HLT_DoubleEl_DZ(): "<<phys.HLT_DoubleEl_DZ()<<endl; }
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



//=============================
// Has Good Event Functions
//=============================

bool ZMETLooper::passNJetsCut()
{
    if(phys.njets() < 2)
        return false;
    return true;
}

bool ZMETLooper::passNBJetsCut()
{
    if(phys.nBJetMedium() < 1)
        return false;
    return true;
}

bool ZMETLooper::passMETCut()
{
    if(phys.met_T1CHS_miniAOD_CORE_pt() < 150)
        return false;
    return true;
}

bool ZMETLooper::passIsotrackCut()
{
    if(phys.nisoTrack_PFLep5_woverlaps() < 3 && phys.nisoTrack_PFHad10_woverlaps() == 0)
        return true;
    return false;
}

bool ZMETLooper::isDuplicate(){
  //cout<<__LINE__<<endl;
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    //cout<<__LINE__<<endl;
    if (is_duplicate(id) ){
      ++nDuplicates;
      //cout<<__LINE__<<endl;
      if (printFail) cout<<phys.evt()<<" :Is a duplicate"<<endl;
      return true;
    }
    //cout<<__LINE__<<endl;
  }
  //cout<<__LINE__<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes not a duplicate"<<endl;
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

  if(!phys.Flag_badChargedCandidateFilter())
  {
      if(printFail) cout<<phys.evt()<<" :Failed BadChargedCandidate filter"<<endl;
      return false;
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

  if(phys.lep_pt().at(0) < 25)
      return false;
  if(phys.lep_pt().at(1) < 20)
      return false;
  if(phys.evt_type() != 0)
      return false;
  if(phys.dRll() < 0.1)
      return false;
  if(phys.dilpt() < 25)
      return false;
  if(phys.dilmass() < 20)
      return false;
  if( abs(phys.lep_p4().at(0).eta()) > 2.4) 
    return false; // eta < 2.4
  if( abs(phys.lep_p4().at(1).eta()) > 2.4)
    return false; // eta < 2.4
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 )
    return false;
  if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 )
      return false; // veto xition region
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


bool ZMETLooper::passEMuSelections()
{

    if (! (phys.evt_passgoodrunlist() > 0))
        return false;
    if(phys.nlep() < 2)
        return false;
    if(phys.hyp_type() != 2)
        return false;
    if(!passEMuTriggers())
        return false;
    if(phys.lep_pt().at(0) < 20)
        return false;
    if(phys.lep_pt().at(1) < 20)
        return false;
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
    g_nBJetMedium = phys.nBJetMedium_up();
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
    g_nBJetMedium = phys.nBJetMedium_dn();
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
    g_nBJetMedium = phys.nBJetMedium();
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
    g_nBJetMedium = phys.nBJetMedium();
    g_met = phys.met_T1CHS_miniAOD_CORE_pt();
    g_met_phi = phys.met_T1CHS_miniAOD_CORE_phi();
    g_mt2 = phys.mt2();
    g_mt2b = phys.mt2b();
    g_ht = phys.ht();
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



void ZMETLooper::setupExternal(TString savePath){
   if(year_fromCommandLine > 0)
   {
     g_year = year_fromCommandLine;
   }
   else
   {
     g_year = 2017;
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
  std::string folderCreation = "mkdir -p "+std::string(savePath.Data());
  system(folderCreation.c_str());
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


  setupExternal(savePath);

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
//===========================================
// File Loop
//===========================================
    
    initSyncFiles(savePath);    
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
    //cout<<__LINE__<<endl;
    //cout<<__LINE__<<endl;
    files_log<<"Running over new file: "<<currentFile->GetTitle()<<endl;
    cout<<"Running over new file: "<<currentFile->GetTitle()<<endl;

    
//===========================================
// Loop over Events in current file
//===========================================
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      phys.GetEntry(event);
      ++nEventsTotal;
      //cout<<__LINE__<<endl;
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );
      setupGlobals();
      //eventsInFile++;
      //if (eventsInFile > 100) continue;
      //cout<<__LINE__<<endl;

      if (conf->get("ETH_Cleanse") != "false"){
          if (phys.isData() && conf->get("event_type") == "dilepton" && (! passETHDileptonDataCleanse()) ){
          //cout<<"Failed ETH Dilepton Data Cleanse"<<endl;
          continue;
        }
      }
      //cout<<__LINE__<<endl;

      if ( isDuplicate() )
      {
        //cout<<"Failed Duplicate"<<endl;
        continue;
      } // check for duplicates
      //cout<<__LINE__<<endl;

      if (! passEMuSelections() )
      {
        continue;
      }
      writeSyncFile(syncFile_emu);

      if (! passBaseCut())
      {
        continue;
      }
      writeSyncFile(syncFile_baseline);

      if(!passMETFilters())
      {
          continue;
      }
      writeSyncFile(syncFile_metFilter);

 

      if(!passNJetsCut())
      {
          continue;
      }
      writeSyncFile(syncFile_njets);

     if(!passMETCut())
      {
          continue;
      }
      writeSyncFile(syncFile_met);

      if(!passIsotrackCut())
      {
          continue;
      }
      writeSyncFile(syncFile_isotracks);

      eventCount++;
  }

 }

  //
  cout<<"Num events passed: "<<eventCount<<endl;

  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
  //cout<<__LINE__<<endl;
//=======================================
// Write Out Histos
//=======================================

  
  bmark->Stop("benchmark");
  //====================
  // Clean Everything Up
  //====================
  closeSyncFiles();
  delete bmark; 
  return 0;
}

void ZMETLooper::closeSyncFiles()
{
    syncFile_baseline->close();
    syncFile_emu->close();
    syncFile_njets->close();
    syncFile_metFilter->close();
    syncFile_met->close();
    syncFile_isotracks->close();
}



