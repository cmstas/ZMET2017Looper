
# include "makePtReweightHisto.h"

using namespace std;

void makePtReweightHisto(ConfigParser * conf,int year)
{
  TString hist_name = "vpt";
  TString output_dir;
  if(conf->get("auto_scale") == "true")
  {
      output_dir = getOutputDir(conf,"allhist",year);
  }
      
  else output_dir = getOutputDir(conf, "hist"); //get output dir location for this histogram something like /nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/prediction/PhotonData_VPTRWT/TChiHZ/
  TString output_location = output_dir+conf->get("Name")+"_vpt_rwt.root";
  system("mkdir -p "+output_dir); 

  //-----------------------------------
  // Get Path to Histograms From Config
  //-----------------------------------
  TString primary_path = output_dir;
  primary_path.ReplaceAll("/reweightGamma/", "/cleanZ/");
  primary_path += (conf->get("vpt_rwt_primary_samplename") != "") ? conf->get("vpt_rwt_primary_samplename")+".root" : "DileptonData.root";

  TString secondary_path = output_dir;
  secondary_path.ReplaceAll("/reweightGamma/", "/cleanGamma/");
  secondary_path += (conf->get("vpt_rwt_secondary_samplename") != "") ? conf->get("vpt_rwt_secondary_samplename")+".root" : conf->get("Name")+".root";

  cout<<"primary_histos: "<<primary_path<<endl;
  cout<<"output_dir: "<<output_dir<<endl;
  cout<<"hist_name: "<<hist_name<<endl;
  cout<<"secondary_path: "<<secondary_path<<endl;
  cout<<"output_location: "<<output_location<<endl;

  //....................
  //Subtraction Hists
  //...................
  vector<TString> subtractor_paths;
  vector<vector<TString>> subtractor_histograms;
  vector<double> subtractor_scales;
  vector<TString> emu_hist = {TString("emu_")+hist_name};
  vector<TString> sf_hist = {TString("ee_")+hist_name,TString("mumu_")+hist_name};
  TString s_path;

  //FS----------------------------
  s_path = primary_path;
//  s_path.ReplaceAll("/cleanZ/", "/FS/");
//  s_path.ReplaceAll("DileptonData.root", "FS.root");
  subtractor_paths.push_back(s_path);

  subtractor_histograms.push_back(emu_hist);
  if (conf->get("old_FS") == "true"){
    subtractor_scales.push_back(1); //RSFOF*kappa
  }
  else{
    subtractor_scales.push_back(0.073905); //RSFOF*kappa
  }

  if (conf->get("signal_region") != "ICHEP"){
    //ZNu---------------------------
    s_path = primary_path;
//    s_path.ReplaceAll("/DileptonData/", "/ZNu/");
    //ZNu-VVV
    s_path.ReplaceAll("Data.root", "VVV.root");
    subtractor_paths.push_back(s_path);
    subtractor_histograms.push_back(sf_hist);
    subtractor_scales.push_back(1);
    //ZNu-ttz
    s_path.ReplaceAll("VVV.root", "TTZ.root");
    subtractor_histograms.push_back(sf_hist);
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1.09);
    //ZNu-zz
    s_path.ReplaceAll("TTZ.root", "ZZ4L.root");
    subtractor_histograms.push_back(sf_hist);
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1.8);
    
    s_path.ReplaceAll("ZZ4L.root","ZZ2L.root");
    subtractor_histograms.push_back(sf_hist);
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(1.8);

    //ZNu-wz
    s_path.ReplaceAll("ZZ2L.root", "WZTo3LNu.root");
    subtractor_histograms.push_back(sf_hist);
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(0.94);

    s_path.ReplaceAll("WZTo3LNu.root","WZTo2L2Q.root");
    subtractor_histograms.push_back(sf_hist);
    subtractor_paths.push_back(s_path);
    subtractor_scales.push_back(0.94);

  }
  cout<<"subtractor_paths: "<<endl;
  for (int i=0; i < (int) subtractor_paths.size(); i++){
    cout<<subtractor_paths.at(i)<<endl;
  }

  TFile * f_primary = TFile::Open(primary_path , "READ"); //typically location to data hist
  TFile * f_secondary = TFile::Open(secondary_path, "READ"); //typically location to zjets hist
  vector<TFile *> f_subtractors;
  for (int i=0; i < (int) subtractor_paths.size(); i++){
    f_subtractors.push_back(TFile::Open(subtractor_paths.at(i), "READ")); //locations of flavor symmetric and ZNu counterparts.
  }

  cout<<"Found input files for reweighting"<<endl;
  TH1D * h_primary;
  TH1D * h_secondary;
  TH1D * h_subtractor;
  TH1D * h_primary_scaled;
  TH1D * h_secondary_scaled;

  TString primary_name = "DileptonData";
  TString secondary_name = "PhotonData";
  if(conf->get("no_subtraction_vpt_rwt") == "true")
  {
    //Did not use DileptonData directly - so will not have ee and mumu seprate
    h_primary = (TH1D*)f_primary->Get(hist_name)->Clone(primary_name);

  }
  else
  {
  	h_primary = (TH1D*)f_primary->Get("ee_"+hist_name)->Clone(primary_name);
 	 h_primary->Add((TH1D*)f_primary->Get("mumu_"+hist_name)->Clone(primary_name+"mumu"));
  }

  h_secondary = (TH1D*)f_secondary->Get(hist_name)->Clone(secondary_name);

  if (conf->get("no_subtraction_vpt_rwt") != "true"){
    cout<<subtractor_histograms.at(0).at(0)<<endl;
    h_subtractor = (TH1D*)(f_subtractors.at(0))->Get(subtractor_histograms.at(0).at(0))->Clone("subtractor_"+primary_name);
    for(size_t iter = 1; iter < subtractor_histograms.at(0).size(); iter++)
    {
        cout<<subtractor_histograms.at(0).at(iter)<<endl;
	h_subtractor->Add((TH1D*)(f_subtractors.at(0)->Get(subtractor_histograms.at(0).at(iter))));
    }
    h_subtractor->Scale(subtractor_scales.at(0));
    for (int i=1; i < (int) subtractor_paths.size(); i++){
      for(auto &histName:subtractor_histograms.at(i))
      {
         cout<<"retrieving "<<histName<<" from "<<subtractor_paths.at(i)<<endl;
	 h_subtractor->Add((TH1D*)(f_subtractors.at(i))->Get(histName), subtractor_scales.at(i));
      }
    }
    cout<<"Retrived Histograms, subtracting other backgrounds"<<endl;
    h_primary->Add(h_subtractor, -1);
  }
  else{
    cout<<"Skipping background subtraction"<<endl;
  }
  cout<<"Zeroing negative bins"<<endl;

  zeroNegatives(h_primary);
  zeroNegatives(h_secondary);

  TH1D * h_ratio_unscaled = (TH1D*) h_primary->Clone(hist_name+"_ratio_unscaled");
  h_ratio_unscaled->Divide(h_secondary);

  h_primary_scaled=(TH1D*) h_primary->Clone(primary_name+"_scaled");
  h_secondary_scaled=(TH1D*) h_secondary->Clone(secondary_name+"_scaled");
  h_secondary_scaled->Scale(1./h_secondary->GetSumOfWeights());
  h_primary_scaled->Scale(1./h_primary->GetSumOfWeights());

  TH1D * h_ratio = (TH1D*) h_primary_scaled->Clone(hist_name+"_ratio");
  h_ratio->Divide(h_secondary_scaled);

  TFile * file = TFile::Open(output_location,"RECREATE");
  file->cd();
  h_ratio->Write();
  h_ratio_unscaled->Write();
  h_primary->Write();
  h_secondary->Write();
  h_primary_scaled->Write();
  h_secondary_scaled->Write();
  file->Close();

  cout<<"Reweight histogram succesfully made at "<<output_location<<endl;
  return ;
}


void makePtReweightHisto_allSR(ConfigParser *conf,int year)
{
  //make reweight histograms for all SRs and VRs

  TString hist_name_suffix = "vpt";
  TString output_dir;
  if(conf->get("auto_scale") == "true")
  {
      output_dir = getOutputDir(conf,"allhist",year);
  }
      
  else output_dir = getOutputDir(conf, "hist"); //get output dir location for this histogram something like /nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/prediction/PhotonData_VPTRWT/TChiHZ/
 
  TString output_location = output_dir + conf->get("Name") + "_vpt_rwt.root";

  TString primary_path = output_dir;

  primary_path.ReplaceAll("/PhotonData_VPTRWT/", "/DileptonData/");
  primary_path += (conf->get("vpt_rwt_primary_samplename") != "") ? conf->get("vpt_rwt_primary_samplename")+".root" : "DileptonData.root";

  TString secondary_path = output_dir;
  secondary_path.ReplaceAll("/PhotonData_VPTRWT/", "/PhotonData/");
  secondary_path += (conf->get("vpt_rwt_secondary_samplename") != "") ? conf->get("vpt_rwt_secondary_samplename")+".root" : conf->get("Name")+".root";


  cout<<"primary_histogram file: "<<primary_path<<endl;
  cout<<"output_dir: "<<output_dir<<endl;
  cout<<"secondary_path: "<<secondary_path<<endl;
  cout<<"output_location: "<<output_location<<endl;

  TFile *f_primary = TFile::Open(primary_path,"READ"); //path to zjets
  TFile *f_secondary = TFile::Open(secondary_path,"READ"); //path to WGammaJets
  cout<<"Found input files for reweighting"<<endl;
  vector<TString> SRs = {"SRA","SRAb","SRB","SRBb","SRC","SRCb","SRVZResolved","SRVZBoosted","SRHZ","VRA","VRB","VRC","VRWZBoosted","VRWZResolved","VRHZ"};

  TH1D * h_primary;
  TH1D * h_secondary;
  TH1D * h_primary_scaled;
  TH1D * h_secondary_scaled;
  vector<TH1D*> h_ratios_unscaled;
  vector<TH1D*> h_ratios;
  vector<TH1D*> h_primaries_scaled;
  vector<TH1D*> h_secondaries_scaled;
  TH1D* h_ratio_unscaled;
  TH1D* h_ratio;
  TString hist_name;
  TString primary_name = "DYJets";
  TString secondary_name = "GammaJets";
  for(auto &it:SRs)
  {
    hist_name = it + hist_name_suffix;

    h_primary = (TH1D*)f_primary->Get(hist_name)->Clone(primary_name);
    h_secondary = (TH1D*)f_secondary->Get(hist_name)->Clone(secondary_name);
    cout<<"Zeroing negative bins"<<endl;

    zeroNegatives(h_primary);
    zeroNegatives(h_secondary);

    h_ratio_unscaled = (TH1D*) h_primary->Clone(hist_name+"_ratio_unscaled");
    h_ratio_unscaled->Divide(h_secondary);
    h_ratios_unscaled.push_back(h_ratio_unscaled);

    h_primary_scaled=(TH1D*) h_primary->Clone(primary_name+"_scaled");
    h_secondary_scaled=(TH1D*) h_secondary->Clone(secondary_name+"_scaled");
    h_secondary_scaled->Scale(1./h_secondary->GetSumOfWeights());
    h_primary_scaled->Scale(1./h_primary->GetSumOfWeights());
    h_primaries_scaled.push_back(h_primary_scaled);
    h_secondaries_scaled.push_back(h_secondary_scaled);

    h_ratio = (TH1D*) h_primary_scaled->Clone(hist_name+"_ratio");
    h_ratio->Divide(h_secondary_scaled);
    h_ratios.push_back(h_ratio);
    }

    TFile * file = TFile::Open(output_location,"RECREATE");
    file->cd();
    for(auto &it:h_ratios)
    {
      it->Write();
    }
    for(auto &it:h_ratios_unscaled)
    {
      it->Write();
    }
    for(auto &it:h_primaries_scaled)
    {
      it->Write();
    }
    for(auto &it:h_secondaries_scaled)
    {
      it->Write();
    }
    file->Close();
  }
