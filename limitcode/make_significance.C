#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (std::getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

void Fill2d(TH2F *&hist, double w, double x, double y){   
  x = std::min(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetLast()) , x); 
  x = std::max(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetFirst()), x); 
  y = std::min(hist->GetYaxis()->GetBinCenter(hist->GetYaxis()->GetLast()) , y); 
  y = std::max(hist->GetYaxis()->GetBinCenter(hist->GetYaxis()->GetFirst()), y); 
  hist->SetBinContent(hist->FindBin(x, y), w); 
}

void make_allRValues(std::string model, std::string list_filename){

  //setup file to hold r-values
  TString outfilename = Form("significances_%s.root", model.c_str()); 
  TFile *f = new TFile(outfilename, "RECREATE");
  
  // default case: T5ZZ binning
  double m1_max = 2425;
  double m1_min = 1025;
  double m2_max = 2425;
  double m2_min = 75;
  int m1_div = 50;
  int m2_div = 50;

  if (model.find("TChiWZ") != std::string::npos) {
    m1_max = 1012.5;
    m1_min = 87.5;
    m2_max = 755;
    m2_min = -5;
    m1_div = 25;
    m2_div = 10;
  }

  int nbinsx = (m1_max - m1_min)/m1_div;
  int nbinsy = (m2_max - m2_min)/m2_div;

  std::cout << "nbinsx: " << nbinsx << ", nbinsy: " << nbinsy << std::endl;

  f->cd();
  TH2F *hObs   = new TH2F("hObs",   "hObs"  , nbinsx, m1_min, m1_max, nbinsy, m2_min, m2_max);

  // read file with list of combine output filenames
  std::ifstream listfile(list_filename.c_str());
  std::string filename;
  while (std::getline(listfile, filename)) {
  
    //This file is created earlier by running combine
    //  TFile *limit_file = new TFile(Form("limit_%s_%d_%d.root", model.c_str(), m1, m2), "READ");
    TFile *limit_file = new TFile(filename.c_str(), "READ");
    TTree *limit_tree = (TTree*)limit_file->Get("limit");

    //This file is created earlier by running combine
    //TFile *significance_file = new TFile(Form("significance_%s_%d_%d.root", model.c_str(), m1, m2), "READ");
    //TTree *significance_tree = (TTree*)significance_file->Get("limit");

    double value = -1.0;
    limit_tree->SetBranchAddress("limit", &value);

    double limit   = -1.0; //observed limit

      limit_tree->GetEntry(0);
      limit   = value;

    //double sig = -1.0;
    //significance_tree->SetBranchAddress("limit", &sig);
    //significance_tree->GetEntry(0);

    delete limit_tree;
    limit_file->Close();
    delete limit_file;

    //delete significance_tree;
    //significance_file->Close();
    //delete significance_file;

    // protect against non-existant mass points
    if( value > 21263 ) continue;

    // get mass point
    //  filenames of the form: limit_TChiWZ_750_300.root
    std::vector<std::string> tokens = split(filename,'_');
    double m1 = std::stod(tokens[2]);
    std::vector<std::string> tokens2 = split(tokens[3],'.');
    double m2 = std::stod(tokens2[0]);

    if (m1 < m1_min || m2 < m2_min) continue;
  
    f->cd();

    Fill2d(hObs  , limit  , m1, m2);  

  } // loop over combine output files
  
  f->cd();
  hObs  ->Write("",TObject::kOverwrite);
  f->Close();
  delete f;

}
