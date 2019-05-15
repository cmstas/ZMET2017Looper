# include "HistTools.h"

void fill1DHistograms(std::string name, float xval, double weight, std::unordered_map<std::string,TH1*> &allHistos,const char *title, int nbins, double xmin, double xmax,TDirectory *rootdir)
{
    if(strcmp(title,"") == 0)
        title = name.c_str();
    if(allHistos[name] == nullptr)
    {
        allHistos[name] = new TH1D(name.c_str(),title,nbins,xmin,xmax);
        allHistos[name]->SetDirectory(rootdir);
        allHistos[name]->Sumw2();
    }

    allHistos[name]->Fill(xval,weight);
}


void fill1DHistograms(std::string name, float xval, double weight, std::unordered_map<std::string, TH1*> &allHistos,const char *title, int nbins,const double *xbins,TDirectory *rootdir)
{
    if(strcmp(title, "") == 0)
        title = name.c_str();
    if(allHistos[name] == nullptr)
    {
        allHistos[name] = new TH1D(name.c_str(),title,nbins,xbins);
        allHistos[name]->SetDirectory(rootdir);
        allHistos[name]->Sumw2();
    }

    allHistos[name]->Fill(xval,weight);
}


void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, double xmin, double xmax, int nbinsy, double ymin, double ymax, TDirectory *rootdir)
{
    if(strcmp(title, "") == 0)
        title = name.c_str();
    if(all2DHistos[name] == nullptr)
    {
        all2DHistos[name] = new TH2D(name.c_str(),title,nbinsx,xmin,xmax,nbinsy,ymin,ymax);
        all2DHistos[name]->SetDirectory(rootdir);
        all2DHistos[name]->Sumw2();
    }

    all2DHistos[name]->Fill(xval,yval,weight);
}

void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, const double *xbins, int nbinsy, double ymin, double ymax, TDirectory *rootdir)
{
    if(strcmp(title, "") == 0)
        title = name.c_str();
    if(all2DHistos[name] == nullptr)
    {
        all2DHistos[name] = new TH2D(name.c_str(),title,nbinsx,xbins,nbinsy,ymin,ymax);
        all2DHistos[name]->SetDirectory(rootdir);
        all2DHistos[name]->Sumw2();

    }
    all2DHistos[name]->Fill(xval,yval,weight);
}

void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, const double *xbins, int nbinsy, const double *ybins, TDirectory *rootdir)
{
    if(strcmp(title, "") == 0)
        title = name.c_str();
    if(all2DHistos[name] == nullptr)
    {
        all2DHistos[name] = new TH2D(name.c_str(),title,nbinsx,xbins,nbinsy,ybins);
        all2DHistos[name]->SetDirectory(rootdir);
        all2DHistos[name]->Sumw2();
    }
    all2DHistos[name]->Fill(xval,yval,weight);
}

void fill2DHistograms(std::string name,float xval, float yval, double weight, std::unordered_map<std::string, TH2*> &all2DHistos, const char *title, int nbinsx, double xmin, double xmax, int nbinsy, const double *ybins, TDirectory *rootdir)
{
    if(strcmp(title, "") == 0)
        title = name.c_str();
    if(all2DHistos[name] == nullptr)
    {
        all2DHistos[name] = new TH2D(name.c_str(),title,nbinsx,xmin,xmax,nbinsy,ybins);
        all2DHistos[name]->SetDirectory(rootdir);
        all2DHistos[name]->Sumw2();

    }
    all2DHistos[name]->Fill(xval,yval,weight);
}



void updateOverUnderflow( TH1D * &hist, double xmax, double xmin = -100000 ){
  /* updates bins at the edges of xmax (xmin) with everything above (below) including over(under)flow */
  int overflowbin = hist->FindBin(xmax-0.0001);
  for( int bini = overflowbin; bini <= hist->GetNbinsX(); bini++ ){
    hist->SetBinContent( overflowbin, hist->GetBinContent( overflowbin ) + hist->GetBinContent( bini + 1 ) ); 
    hist->SetBinError  ( overflowbin, sqrt( pow(hist->GetBinError  ( overflowbin ), 2 ) + pow( hist->GetBinError( bini + 1 ), 2 ) ) );  
    hist->SetBinContent( bini + 1, 0 );
    hist->SetBinError  ( bini + 1, 0 );
  }

  if (xmin > -100000){
    int underflowbin = hist->FindBin(xmin+0.0001);
    for(int bini = overflowbin; bini > 0; bini-- ){
      hist->SetBinContent( underflowbin, hist->GetBinContent( underflowbin ) + hist->GetBinContent( bini - 1 ) ); 
      hist->SetBinError  ( underflowbin, sqrt( pow(hist->GetBinError  ( underflowbin ), 2 ) + pow( hist->GetBinError( bini - 1 ), 2 ) ) );  
      hist->SetBinContent( bini - 1, 0 );
      hist->SetBinError  ( bini - 1, 0 );
    }
  }
}

void blindAfter(TH1D * &hist, double xmax){
  /* Sets all the bins starting from xmax to 0, count and error */
  int max_bin = hist->FindBin(xmax);
  for( int bini = max_bin; bini <= hist->GetNbinsX()+1; bini++ ){
    hist->SetBinContent( bini, 0 ); 
    hist->SetBinError  ( bini, 0 );  
  }
}

void zeroNegatives(TH1D* &hist){
  /* Zeros all the bins with negative weights */
  for( int i = 0; i <= hist->GetNbinsX()+1; i++ ){
    if ( ((double) hist->GetBinContent(i)) < 0 ){
      hist->SetBinContent( i, 0 );
    }
  }
}

