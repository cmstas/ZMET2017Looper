#include "TChain.h"
#include "TString.h"
#include "ConfigHelper.C"

void addToChain(TChain *ch, TString set) {

//====================================
// DY MC
//====================================

  if (set == "ZMC-inclusive"){
    //Inclusive HT
    cout<<"Adding ZMC-inclusive"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ext1*");
  }
  else if (set == "ZMC-100200"){
    //HT 100-200 
    cout<<"Adding ZMC-100200"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht100_ext1*");
  }
  else if (set == "ZMC-200400"){
    //HT 200-400 
    cout<<"Adding ZMC-200400"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht200_ext1*");
  }
  else if (set == "ZMC-400600"){
    //HT 400-600 
    cout<<"Adding ZMC-400600"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht400_ext1*");
  }
  else if (set == "ZMC-600Inf"){
    //HT 600-Inf 
    cout<<"Adding ZMC-600Inf"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht600_ext1*");
  }
  else if (set == "ZMC-fullmc"){
    cout<<"Adding ZMC-fullmc"<<endl;
    
    //ZZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2q_amcnlo*.root" ));
    //WZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_2l2q_amcnlo*.root" ));
  }
  else if (set == "ZMC-low-mass"){
    cout<<"Adding ZMC Low Mass sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m1050_mgmlm*.root");
  }
  //---------------------------
  // DY Skims
  //---------------------------
  else if (set == "ZMC-inclusive-Skimmed"){
    //Inclusive HT
    cout<<"Adding ZMC-inclusive-Skimmed"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ext1*");
  }
  else if (set == "ZMC-100200-Skimmed"){
    //HT 100-200 
    cout<<"Adding ZMC-100200-Skimmed"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht100_ext1*");
  }
  else if (set == "ZMC-200400-Skimmed"){
    //HT 200-400 
    cout<<"Adding ZMC-200400-Skimmed"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht200_ext1*");
  }
  else if (set == "ZMC-400600-Skimmed"){
    //HT 400-600 
    cout<<"Adding ZMC-400600-Skimmed"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht400_ext1*");
  }
  else if (set == "ZMC-600Inf-Skimmed"){
    //HT 600-Inf 
    cout<<"Adding ZMC-600Inf-Skimmed"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht600_ext1*");
  }
  else if (set == "ZMC-fullmc-Skimmed"){
    cout<<"Adding ZMC-fullmc-Skimmed"<<endl;
    
    //ZZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/zz_2l2q_amcnlo*.root" ));
    //WZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/wz_2l2q_amcnlo*.root" ));
  }
  else if (set == "ZMC-low-mass-Skimmed"){
    cout<<"Adding ZMC Low Mass Skimmed sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m1050_mgmlm*.root");
  }

  else if (set == "ZMC-V08-22-08-Skimmed"){
    cout<<"Adding V08-22-08 Skimmed sample"<<endl;
    
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ext1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht100*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht200*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht400*"); 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht600*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht800*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht1200*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m50_mgmlm_ht2500*");

    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/dy_m1050_mgmlm*.root");

    //ZZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/zz_2l2q_amcnlo*.root" ));
    //WZ Sample
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/wz_2l2q_amcnlo*.root" ));
  }

//====================================
// SUSY MC
//====================================

  else if (set == "TChiHZ"){
    //TChiHZ Signal
    cout<<"Adding TChiHZ Monte Carlo"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-02/tchihz_80x_v2*.root");
  }
  else if (set == "T5ZZ"){
    //TChiHZ Signal
    cout<<"Adding T5ZZ Monte Carlo"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-07/t5zz_80x_v2*.root");
  }

//====================================
// Gamma Jets MC
//====================================

  else if (set == "GammaMC-40100"){
    //HT 40-100
    cout<<"Adding GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht40_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-100200"){
    //HT 100-200 
    cout<<"Adding GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht100_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-200400"){
    //HT 200-400
    cout<<"Adding GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht200_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-400600"){
    //HT 400-600 
    cout<<"Adding GammaMC-400600"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht400_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-600Inf"){
    //HT 600-Inf 
    cout<<"Adding GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht600_dr0p05_mgmlm*");
  }
  //---------------------------
  // Gamma Jets HT Binned Skims
  //---------------------------
  else if (set == "GammaMC-40100-Skimmed"){
    //HT 40-100
    cout<<"Adding Skimmed GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht40_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-100200-Skimmed"){
    //HT 100-200 
    cout<<"Adding Skimmed GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht100_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-200400-Skimmed"){
    //HT 200-400
    cout<<"Adding Skimmed GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht200_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-400600-Skimmed"){
    //HT 400-600 
    cout<<"Adding Skimmed GammaMC-400600"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht400_dr0p05_mgmlm*");
  }
  else if (set == "GammaMC-600Inf-Skimmed"){
    //HT 600-Inf 
    cout<<"Adding Skimmed GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht600_dr0p05_mgmlm*");
  }
  //---------------------------
  // dr0p4
  //---------------------------
  else if (set == "GammaMC-40100-dr0p4-Skimmed"){
    //HT 40-100
    cout<<"Adding Skimmed GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht40_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-100200-dr0p4-Skimmed"){
    //HT 100-200 
    cout<<"Adding Skimmed GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht100_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-200400-dr0p4-Skimmed"){
    //HT 200-400
    cout<<"Adding Skimmed GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht200_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-400600-dr0p4-Skimmed"){
    //HT 400-600 
    cout<<"Adding Skimmed GammaMC-400600"<<endl; 
    ch->Add("/hadoop/cms/store/user/olivito/AutoTwopler_babies/merged/ZMET/V08-22-09/output/gjetsht400_dr0p4_mgmlm*");
  }
  else if (set == "GammaMC-600Inf-dr0p4-Skimmed"){
    //HT 600-Inf 
    cout<<"Adding Skimmed GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-08/skims/gjetsht600_dr0p4_mgmlm*");
  }
  //---------------------------
  // Other Gamma MC
  //---------------------------
  else if (set == "GammaMC-justWJets"){
    cout<<"Adding GammaMC-wjets-inclusive"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");
  }
  else if (set == "GammaMC-WGamma"){
    //This set combines WJets with WGJets and is intended to take only prompt photons from WGjets and only non-prompt photons from WJets

    cout<<"Adding GammaMC-WGamma"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
  }
  else if (set == "GammaMC-WGamma-Skimmed"){
    //This set combines WJets with WGJets and is intended to take only prompt photons from WGjets and only non-prompt photons from WJets

    cout<<"Adding Skimmed GammaMC-WGamma"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_incl_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht100_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht200_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht400_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht600_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht800_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht1200_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht2500_mgmlm*");

    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wgjets_incl_mgmlm*");
  }
  else if (set == "GammaMC-gammaZtoNuNu"){
    cout<<"Adding GammaMC-gammaZtoNuNu"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma*.root");
  }
  else if (set == "GammaMC-gammaZtoNuNu-Skimmed"){
    cout<<"Adding Skimmed GammaMC-gammaZtoNuNu"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_znunugamma*.root");
  }
  else if (set == "GammaMC-wgjets-inclusive-amc"){
    cout<<"Adding GammaMC WGJets AMC@NLO Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo*.root");
  }
  else if (set == "GammaMC-wgjets-inclusive-mgmlm"){
    cout<<"Adding GammaMC WGJets Madgraph Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_mgmlm*.root");
  }
  else if (set == "GammaMC-GammaRealMET"){
    cout<<"Adding GammaMC-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }

//====================================
// Flavor Symmetric MC
//====================================

  else if (set == "FSMC-dilep-mgmlm"){
    cout<<"Adding FSMC-dilep-mgmlm (smaller stats sample)"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
  }
  else if (set == "FSMC-dilep-powheg"){
    cout<<"Adding FSMC-dilep-powheg (large stats sample)"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_powheg*.root")); //larger stats
  }
  else if (set == "FSMC-dilep-powheg-Skimmed"){
    cout<<"Adding Skimmed FSMC-dilep-powheg (large stats sample)"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_dilep_powheg*.root")); //larger stats
  }
  else if (set == "FSMC-TTBar-NoPromptGamma"){
    cout<<"Adding FSMC-TTBar-NoPromptGamma (large stats sample)"<<endl; 
    //Take non-prompt photon events
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_dilep_powheg*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltbr_mgmlm*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltop_mgmlm*.root"));
  }
  else if (set == "FSMC-TTGamma-NoNonPromptGamma"){
    cout<<"Adding FSMC-TTGamma-NoNonPromptGamma"<<endl; 
    //Take prompt photon events
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttg_incl_amcnlo*.root"));
  }
  else if (set == "FSMC-1lep"){
    cout<<"Adding FSMC-1lep"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root"));
  }
  else if (set == "FSMC-1lep-Skimmed"){
    cout<<"Adding skimmed FSMC-1lep"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltbr_mgmlm_ext1*"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-09/skims/ttbar_1ltop_mgmlm_ext1*"));
  }
  else if (set == "FSMC-singletop-inclusive"){
    cout<<"Adding FSMC-singletop"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_powheg*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_powheg*.root"));
  }
  else if (set == "FSMC-singletop-nofullhaddecay"){
    cout<<"Adding FSMC-singletop with no fully hadronic decay"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_top_nofullhaddecay_powheg.root"));
  }
  else if (set == "FSMC-singletop-nofullhaddecay-Skimmed"){
    cout<<"Adding Skimmed FSMC-singletop with no fully hadronic decay"<<endl; 
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_sttw_top_nofullhaddecay_powheg.root"));
  }
  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;
  else if (set == "FSMC-ww"){
    cout<<"Adding FSMC-ww"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ww_2l2nu_powheg*.root"));
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/www_incl_amcnlo*.root");
  }
  else if (set == "FSMC-ww-Skimmed"){
    cout<<"Adding FSMC-ww-Skimmed"<<endl; 
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ww_2l2nu_powheg*.root"));
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_www_incl_amcnlo*.root");
  }
  else if (set == "FSMC-ttw"){
    cout<<"Adding FSMC-ttw"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_ln_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_qq_amcnlo*.root");
  }
  else if (set == "FSMC-ttw-Skimmed"){
    cout<<"Adding FSMC-ttw-Skimmed"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttw_ln_amcnlo*.root");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttw_qq_amcnlo*.root");
  }

//====================================
// Rare MC
//====================================

  else if (set == "RareMC-wz"){
    cout<<"Adding RareMC-wz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_3lnu_powheg*.root");
  }
  else if (set == "RareMC-zz"){
    cout<<"Adding RareMC-zz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2n_powheg*.root");
  }
  else if (set == "RareMC-vvv"){
    cout<<"Adding RareMC-vvv"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wwz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wzz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zzz_incl_amcnlo*.root");
  }
  else if (set == "RareMC-ttz"){
    cout<<"Adding RareMC-ttz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_2l2n_amcnlo_ext*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_qq_amcnlo*.root");
  } 
  else if (set == "RareMC-wz-Skimmed"){
    cout<<"Adding RareMC-wz skim"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wz_3lnu_powheg*.root");
  }
  else if (set == "RareMC-zz-Skimmed"){
    cout<<"Adding RareMC-zz skim"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_zz_2l2n_powheg*.root");
  }
  else if (set == "RareMC-vvv-Skimmed"){
    cout<<"Adding RareMC-vvv skim"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wwz_incl_amcnlo*.root");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wzz_incl_amcnlo*.root");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_zzz_incl_amcnlo*.root");
  }
  else if (set == "RareMC-ttz-Skimmed"){
    cout<<"Adding RareMC-ttz skim"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttz_2l2n_amcnlo_ext*.root");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttz_qq_amcnlo*.root");
  } 

//====================================
// Photon Data
//====================================

  //Single Photon Trigger
  else if (set == "GammaData-SinglePhoton"){
    cout<<"Adding GammaData-SinglePhoton"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_ph_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v3*");
    
    //EWK Subtraction
  }
  else if (set == "GammaData-EWKSub"){
    cout<<"Adding EWK Subtraction Samples"<<endl;       
    //============
    // W+Gamma+Jets
    //============
    //This is the Wjets sample, it is intended to have events with a prompt photon vetod
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

    //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
    //============
    // Gamma+Z->NuNu
    //============
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma_ptg130_mgmlm*.root");
    //============
    // TTbar
    //============
    //1lep
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root");
    //dilep
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
    //============
    // Single Top
    //============
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_nofullhaddecay_powheg.root"));
  }
  else if (set == "GammaData-GammaRealMET"){
    cout<<"Adding GammaData-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  else if (set == "GammaData-JustWjetRealMET"){
    cout<<"Adding GammaData-JustWjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  else if (set == "GammaData-JustWGjetRealMET"){
    cout<<"Adding GammaData-JustWGjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
  }
  else if (set == "GammaData-JustTTBarRealMET"){
    cout<<"Adding GammaData-JustTTBarRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
  }

//====================================
// Leptonic Data
//====================================

  else if (set == "DileptonData-ee"){
    cout<<"Adding EE Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_ee_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ee_v3*");
  }
  else if (set == "DileptonData-em"){
    cout<<"Adding EMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_em_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_em_v3*");
  }
  else if (set == "DileptonData-mm"){
    cout<<"Adding MuMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_mm_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_mm_v3*");
  }
  else if (set == "SingleLeptonData-SingleMu"){
    cout<<"Adding Single Muon Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_sm_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_sm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_sm_v3*");
  }

//====================================
// Skimmed Leptonic Data
//====================================

  else if (set == "DileptonData-ee-Skimmed"){
    cout<<"Adding Skimmed EE Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_ee_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ee_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ee_v3*");
  }
  else if (set == "DileptonData-em-Skimmed"){
    cout<<"Adding Skimmed EMu Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_em_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_em_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_em_v3*");
  }
  else if (set == "DileptonData-mm-Skimmed"){
    cout<<"Adding Skimmed MuMu Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_mm_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_mm_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_mm_v3*");
  }
  else if (set == "SingleLeptonData-SingleMu-Skimmed"){
    cout<<"Adding Skimmed Single Muon Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_sm_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_sm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_sm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_sm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_sm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_sm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_sm_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_sm_v3*");
  }

//====================================
// Skimmed Photon Data
//====================================

  //Single Photon Trigger
  else if (set == "GammaData-SinglePhoton-Skimmed"){
    cout<<"Adding GammaData-SinglePhoton-Skimmed"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_ph_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ph_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ph_v3*");
  }
  //EWK Subtraction
  else if (set == "GammaData-EWKSub-Skimmed"){
    cout<<"Adding Skimmed EWK Subtraction Samples"<<endl;       
    //============
    // W+Gamma+Jets
    //============
    //This is the Wjets sample, it is intended to have events with a prompt photon vetod
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_incl_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht100_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht200_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht400_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht600_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht800_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht1200_mgmlm*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wjets_ht2500_mgmlm*");

    //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_wgjets_incl_mgmlm*");
    //============
    // Gamma+Z->NuNu
    //============
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_znunugamma_ptg130_mgmlm*.root");
    //============
    // TTbar
    //============
    //1lep
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttbar_1ltbr_mgmlm*.root");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttbar_1ltop_mgmlm*.root");
    //dilep
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_ttbar_dilep_mgmlm*.root"));
    //============
    // Single Top
    //============
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_sttw_top_nofullhaddecay_powheg.root"));
  }

//====================================
// End Building TChain
//====================================

  return;
} 


TChain * getTChain(TString data_set) {
  TChain *ch = new TChain("t");

  vector<TString> sets;

  if (data_set[0]=='['){
     sets = sParseVector(data_set);
  }
  else{
    TString token;
    Ssiz_t from=0;
    //cout<<"got vector in string form: "<<opt<<endl;
    while(data_set.Tokenize(token, from, "_")){
      token.ReplaceAll("_", "");
      sets.push_back(TString(token.Data()));
    }
  }

  cout<<"Datasets Incoming: "<<endl;
  cout<<"===================================="<<endl;
  for (std::vector<TString>::iterator i=sets.begin(); i != sets.end(); i++){
    addToChain(ch, *i);
  }
  cout<<"===================================="<<endl;
  
  return ch;
}