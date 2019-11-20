# include "getEWKErrorNums.h"
using namespace std;
vector<TString> getFileLocation(std::string basePath,TString sample_name){
  vector<TString> fnames;

  if(sample_name == "all")
  {
        cout<<"all SR"<<endl;
        if(basePath == "")
        {
            cout<<"EWK histograms base path not given!"<<endl; 
            fnames.push_back("/home/users/bsathian/histsthreeyears/dataTemplates/combined/GammaData_EWKSub.root");
            fnames.push_back("/home/users/bsathian/histsthreeyears/dataTemplates/combined/GammaData.root");
        }
        else
        {
	        fnames.push_back(basePath+"/GammaData_EWKSub.root");
            fnames.push_back(basePath+"/GammaData.root");
        }
  }

  return fnames;
}

pair<double, vector<double>> getEWKNumsForSample(TString sample_name,bool allSR,std::string basePath){
  cout<<setprecision(15);
  vector<TString> fnames;
  if(!allSR)
      fnames = getFileLocation(basePath,sample_name);
  else
      fnames = getFileLocation(basePath,"all");


  //cout<<__LINE__<<endl;

  TFile* sub_file = TFile::Open(fnames[0]);
  TFile* no_sub_file = TFile::Open(fnames[1]);

  //cout<<__LINE__<<endl;
  TH1D *sub_hist, *no_sub_hist;
  if(!allSR)
  {
    sub_hist = (TH1D*) ((TH1D*) sub_file->Get("type1MET"));
    no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get("type1MET"));
  }
  else
  {
      sub_hist = (TH1D*) ((TH1D*) sub_file->Get(sample_name+TString("type1MET")));
      no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get(sample_name+TString("type1MET")));
  }


  vector<double> bins, noSubNums;
  double lowbin_withEwkSub;

  //cout<<__LINE__<<endl;

  if(sample_name == "Strong_Bveto_6j" || sample_name == "Strong_Btag_6j" || sample_name == "baseline" || sample_name == "VRC"){
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(6001);
  }
  else if(sample_name == "TChiWZ"){
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(250);
    bins.push_back(350);
    bins.push_back(6001);
  }
  else{
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(230);
    bins.push_back(300);
    bins.push_back(6001);
  }

  sub_hist = (TH1D*) sub_hist->Rebin(bins.size()-1, TString("sub_rebin"), &bins[0]);
  no_sub_hist = (TH1D*) no_sub_hist->Rebin(bins.size()-1, TString("sub_rebin"), &bins[0]);

  zeroNegatives(sub_hist);
  zeroNegatives(no_sub_hist);

  cout<<"Deriving EWK Subtraction numbers:"<<endl;

  double count_in_sub, count_in_no;
  for (int i = 1; i<=sub_hist->GetNbinsX(); i++){
    count_in_sub = sub_hist->GetBinContent(i);
    count_in_no = no_sub_hist->GetBinContent(i);

    cout<<"bin: "<<bins[i-1]<<"-"<<bins[i]<<" sub: "<<count_in_sub<<" no sub: "<<count_in_no<<" diff: "<<count_in_no - count_in_sub<<endl;

    if (i == 2){
      lowbin_withEwkSub = count_in_sub;
    }

    noSubNums.push_back(count_in_no);
  }

  return make_pair(lowbin_withEwkSub, noSubNums);
}

vector<double> getPercentStatErrorsForNoEWKSub(TString sample_name,bool allSR,std::string basePath){
  cout<<setprecision(15);
  vector<TString> fnames;
  if(!allSR)
      fnames = getFileLocation(basePath,sample_name);
  else
      fnames = getFileLocation(basePath,"all");

  //cout<<__LINE__<<endl;

  TFile* no_sub_file = TFile::Open(fnames[1]);

  //cout<<__LINE__<<endl;
  TH1D * no_sub_hist;
  if(!allSR)
      no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get("type1MET"));
  else
      no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get(sample_name+TString("type1MET")));


  vector<double> bins, noSubCounts, noSubErrs;
  double err_bin;

  //cout<<__LINE__<<endl;

  if(sample_name == "SRC" || sample_name == "SRCb"){
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(6001);
  }
  else if(sample_name == "SRVZResolved"){
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(250);
    bins.push_back(350);
    bins.push_back(6001);
  }
  else if(sample_name == "SRVZBoosted'")
  {
      bins.push_back(50);
      bins.push_back(100);
      bins.push_back(200);
      bins.push_back(300);
      bins.push_back(400);
      bins.push_back(500);
      bins.push_back(6001);
  }
  else if(sample_name == "SRHZ")
  {
      bins.push_back(50);
      bins.push_back(100);
      bins.push_back(150);
      bins.push_back(250);
      bins.push_back(6001);
  }
  else{
    //bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(230);
    bins.push_back(300);
    bins.push_back(6001);
  }
  cout<<"Deriving NoEWKSub stat errors:"<<endl;

  zeroNegatives(no_sub_hist);

  for (int i = 0; i<(int)bins.size()-1; i++){
    noSubCounts.push_back(no_sub_hist->IntegralAndError(bins[i],bins[i+1] - 0.01, err_bin));
    noSubErrs.push_back(err_bin);
  }

  for (int i = 0; i<(int)noSubErrs.size(); i++){
    noSubErrs[i] = noSubErrs[i]/noSubCounts[i];
  }
  return noSubErrs;
}


void getEWKErrorNums(){
  vector<TString> SR;

  SR.push_back("Strong_Btag_2j");
  SR.push_back("Strong_Btag_4j");
  SR.push_back("Strong_Btag_6j");
  SR.push_back("Strong_Bveto_2j");
  SR.push_back("Strong_Bveto_4j");
  SR.push_back("Strong_Bveto_6j");
  SR.push_back("TChiHZ");

  for(vector<TString>::iterator it = SR.begin(); it != SR.end(); ++it) {
    cout<<"==========="<<endl;
    getEWKNumsForSample(*it);
 }
}
