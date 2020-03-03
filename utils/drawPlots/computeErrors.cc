# include "computeErrors.h"
using namespace std;

double err_divide(double A, double B, double errA, double errB) {
  /* A = data yeild in norm bin
  B = template yield in norm bin
  errA/errB = statistical error in those bins*/
  return sqrt((A/B)*(A/B)*(pow(errA/A,2) + pow(errB/B,2)));
}

void printTemplatesDebug(const vector<double> &prediction, const vector<double> &prediction_err, const vector<double> &stat_err, const vector<double> &closure_err, const vector<double> &norm_err, const vector<double> &ewk_err, const vector<pair<double, double>> &bin_edge){
  /* Prints a latex table of the sources of error that go into the templates */
  cout<<fixed;
  cout<<setprecision(2);

  cout<<"TEMPLATEDEBUG: \\begin{tabular} {l | l | l | l | l | l}"<<endl;
  cout<<"TEMPLATEDEBUG: MET Bin & Prediction & Closure (ratio) & Normalization (ratio) & Statistical (ratio) & EWK Sub (ratio) \\\\ \\hline"<<endl;
  for (size_t i = 0; i<bin_edge.size(); i++){
    cout<<"TEMPLATEDEBUG: "<<(int) bin_edge[i].first<<"-"<<(int) bin_edge[i].second<<" & "<<prediction[i]<<" $\\pm$ "<<prediction_err[i]<<" & "<<closure_err[i]<<" ("<<closure_err[i]/prediction_err[i]<<") & "<<norm_err[i]<<" ("<<norm_err[i]/prediction_err[i]<<") & "<<stat_err[i]<<" ("<<stat_err[i]/prediction_err[i]<<") & "<<ewk_err[i]<<" ("<<ewk_err[i]/prediction_err[i]<<") \\\\";
    if (i == bin_edge.size() -1 ){
      cout<<" \\hline";
    }
    cout<<endl;
  }
  cout<<"TEMPLATEDEBUG: \\end{tabular}"<<endl;
}

vector<double> getMetTemplatesError(const vector<double> &stat_err, const vector<double> &bin_count, double normalization, int norm_bin, const vector<pair<double, double>> &bin_edge, TString SR,bool allSR,std::string EWK_base_path){
    cout<<"Computing MET templates systematics"<<endl;
  /* stat_err == statistical error on the template bins
     bin count == bin count on template bins
     normalization == bin count to which the sample was normalized
     normalization_bg == bin count of the templates which was normalized to 'normalization'
     SR == name of signal region */
  vector<double> output_errors;

  cout<<"Computing data/MC normalization errors...."<<endl;
  cout<<"Data count="<<normalization<<" "<<"Template MC count"<<bin_count.at(norm_bin)<<endl;
  cout<<"Normalization Factor for templates from bin "<<norm_bin<<": "<<normalization/bin_count.at(norm_bin)<<endl;

  vector<double> noSubStatErrs=getPercentStatErrorsForNoEWKSub(SR,allSR,EWK_base_path);

  for (size_t i=0; i<=noSubStatErrs.size(); i++){
    noSubStatErrs[i] = noSubStatErrs[i]*bin_count[i];
  }

  //normalization = err_mult(normalization, bin_count[norm_bin], sqrt(normalization), stat_err[norm_bin]);
  normalization = err_divide(normalization, bin_count[norm_bin], sqrt(normalization), noSubStatErrs[norm_bin]);

  //=========
  // Input EWK and Closure Errors
  //=========

  vector<double> No_EWK_BinCount;

  pair<double, vector<double>> EWK_Data;
  double EWK_LowBin;
  vector<double> MC_Closure_Error;

  if (SR == "SRA")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.000); //50-100
    MC_Closure_Error.push_back(.091);  //100-150
    MC_Closure_Error.push_back(.051);  //150-230
    MC_Closure_Error.push_back(.442); //230-300
    MC_Closure_Error.push_back(.810);  //300+
  }
  else if(SR == "SRAb")
  {
    //MC_Closure_Error.push_back(0.00); //0-50
    MC_Closure_Error.push_back(0.000); //50-100
    MC_Closure_Error.push_back(0.067); //100-150
    MC_Closure_Error.push_back(0.120); //150-230
    MC_Closure_Error.push_back(0.462); //230-300
    MC_Closure_Error.push_back(0.439); //300+
  }
  else if(SR == "SRB")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.000); //50-100
    MC_Closure_Error.push_back(.021); //100-150
    MC_Closure_Error.push_back(.057); //150-230
    MC_Closure_Error.push_back(0.169); //230-300
    MC_Closure_Error.push_back(0.169); //300+
  }
  else if(SR == "SRBb")
  {
    //MC_Closure_Error.push_back(0.00); //0-50
    MC_Closure_Error.push_back(0.000); //50-100
    MC_Closure_Error.push_back(0.308); //100-150
    MC_Closure_Error.push_back(0.302); //150-230
    MC_Closure_Error.push_back(0.387); //230-300
    MC_Closure_Error.push_back(0.387); //300+
  }
  else if (SR == "SRC")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.096);  //100-150
    MC_Closure_Error.push_back(.157); //150-250
    MC_Closure_Error.push_back(.554);  //250+
  }
  else if(SR == "SRCb")
  {
  //MC_Closure_Error.push_back(0.00); //0-50
    MC_Closure_Error.push_back(0.00); //50-100
    MC_Closure_Error.push_back(0.095); //100-150
    MC_Closure_Error.push_back(0.166); //150-250
    MC_Closure_Error.push_back(0.696); //250+
  }
  else if(SR == "SRVZBoosted")
  {
      //MC_Closure_Error.push_back(0.00); //0-50
      MC_Closure_Error.push_back(0.00); //50-100
      MC_Closure_Error.push_back(0.298); //100-200
      MC_Closure_Error.push_back(0.298); //200-300
      MC_Closure_Error.push_back(0.298); //300-400
      MC_Closure_Error.push_back(0.298); //400-500
      MC_Closure_Error.push_back(0.298); //500+
    
  }
  else if(SR == "SRVZResolved")
  {
    //MC_Closure_Error.push_back(0.00); //0-50
    MC_Closure_Error.push_back(0.00); //50-100
    MC_Closure_Error.push_back(0.107); //100-150
    MC_Closure_Error.push_back(0.822); //150-250
    MC_Closure_Error.push_back(0.700); //250-350
    MC_Closure_Error.push_back(0.700); //350+
  }
  else if(SR == "SRHZ")
  {
      MC_Closure_Error.push_back(0.00); //50-100
      MC_Closure_Error.push_back(0.338); //100-150
      MC_Closure_Error.push_back(1.796); //150-250
      MC_Closure_Error.push_back(1.796); //250+ 
      
  }
  else if(SR == "VRA")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.067); //100-150
    MC_Closure_Error.push_back(.097); //150-230
    MC_Closure_Error.push_back(0.078); //230-300
    MC_Closure_Error.push_back(.157); //250+
  }
  else if(SR == "VRB")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.068); //100-150
    MC_Closure_Error.push_back(.049); //150-230
    MC_Closure_Error.push_back(0.212); //230-300
    MC_Closure_Error.push_back(0.212); //300+
  }
  else if(SR == "VRC")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.038); //100-150
    MC_Closure_Error.push_back(0.065); //150-250
    MC_Closure_Error.push_back(0.195); //250+
  }
  else if(SR == "VRWZBoosted")
  {
    //MC_Closure_Error.push_back(0.00); //0-50
    MC_Closure_Error.push_back(0.00); //50-100
    MC_Closure_Error.push_back(0.282); //100-200
    MC_Closure_Error.push_back(0.282); //200-300
    MC_Closure_Error.push_back(0.282); //300-400
    MC_Closure_Error.push_back(0.282); //400-500
    MC_Closure_Error.push_back(0.282); //500+
  }
  else if(SR == "VRWZResolved")
  {
     //MC_Closure_Error.push_back(0.00); //0-50
     MC_Closure_Error.push_back(0.00); //50-100
     MC_Closure_Error.push_back(0.220); //100-150
     MC_Closure_Error.push_back(0.084); //150-250
     MC_Closure_Error.push_back(0.147); //250-350
     MC_Closure_Error.push_back(0.147); //350+
  }
  else if(SR == "VRHZ")
  {
    //MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.356); //100-150
    MC_Closure_Error.push_back(.464); //150-250
    MC_Closure_Error.push_back(.464); //250+
  }
  else if(SR == "2j"){
    MC_Closure_Error.push_back(.00); //0-50
    MC_Closure_Error.push_back(.00); //50-100
    MC_Closure_Error.push_back(.50); //100-150
    MC_Closure_Error.push_back(.50); //150-250
    MC_Closure_Error.push_back(.50); //250+
  }
  else if(SR == "baseline"){
    //MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
  }
  else if(SR == "Edge"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.128); //100-150
    MC_Closure_Error.push_back(.368); //150-350
    MC_Closure_Error.push_back(0.641); //350-600
  }
  else{
    std::stringstream message;
    message<<"Closure systematics : Invalid or missing SR set in config. Please check config variable \"SR\". (got '"<<SR<<"').";
    throw std::invalid_argument(message.str());
  }

  EWK_Data = getEWKNumsForSample(SR,allSR,EWK_base_path);
  EWK_LowBin = EWK_Data.first;
  No_EWK_BinCount = EWK_Data.second;

  //The following are two ways to normalize the sample.
  //The first derives the scale factor for the EWK subtracted sample.
  //The second derives it for the non EWK subtracted sample.
  //The first is only there for testing purposes (it's how Vince does it).
  //double EWK_Norm = bin_count[0]/EWK_LowBin;
  //double EWK_Norm = bin_count[0]/No_EWK_BinCount[0];
  double EWK_Norm = bin_count[norm_bin]/No_EWK_BinCount[norm_bin];

  cout<<"Normalizing in bin "<<norm_bin<<" Normalization Target/Num in EWK Sub Sample: "<<bin_count[norm_bin]<<" Num in NonEWKSub Sample: "<<No_EWK_BinCount[norm_bin]<<endl;
  cout<<"Derived scale factor "<<EWK_Norm<<" for Non EWK sample"<<endl;

  double err_bin; //error in bin

  vector<double> closure_err, norm_err, ewk_err;

  for (size_t i=0; i<stat_err.size(); i++){

    //ewk_err.push_back(0.3*abs(bin_count[i] - EWK_Norm*No_EWK_BinCount[i]));
    ewk_err.push_back(0.3/(1-0.3) * abs(bin_count[i] - EWK_Norm * No_EWK_BinCount[i]));
    //cheap hack - we now subtract only 0.7 * EWK
    norm_err.push_back(normalization*bin_count[i]);
    closure_err.push_back(bin_count[i]*MC_Closure_Error[i]);

    cout<<"TRACE| Bin "<<i<<" ";

    cout<<" bin Count: "<<bin_count[i];
    cout<<" EWK Subtraction: "<<ewk_err[i];
    cout<<" Stat Error: "<< stat_err[i];
    cout<<" Stat Error (noEwkSub): "<< noSubStatErrs[i];
    cout<<" Closure Error: "<<closure_err[i];
    cout<<" Normalization: "<<norm_err[i];

    err_bin = stat_err[i]*stat_err[i]; //Statistical Error
    //err_bin = noSubStatErrs[i]*noSubStatErrs[i]; //Statistical Error
    err_bin += bin_count[i]*bin_count[i]*MC_Closure_Error[i]*MC_Closure_Error[i]; //Closure Error
    err_bin += normalization*bin_count[i]*normalization*bin_count[i]; //Normalization of Zjets
    cout<<" Stat+Norm+Closure "<<sqrt(err_bin);
    err_bin += ewk_err[i]*ewk_err[i]; //EWK Subtraction
    cout<<" Stat+Norm+Closure+EWK: "<<sqrt(err_bin)<<endl;

    output_errors.push_back(sqrt(err_bin));
  }

  printTemplatesDebug(bin_count, output_errors, stat_err, closure_err, norm_err, ewk_err, bin_edge);

  cout<<setprecision(10);
  //--------------------------------
  // To be parsed by datacard maker
  //--------------------------------
  cout<<"{zjets_norm} "<<1.+normalization<<endl;

  double bin_count_safe = 1; //Don't divide by zero even when bin count is 0.

  for (int i = 0; i<(int)bin_count.size(); i++){
    bin_count_safe = bin_count[i];
    cout<<"1-bin_count[i]"<<1-bin_count[i]<<endl;
    if (bin_count[i] == 0){
      cout<<"bin_count_[i] is 0"<<endl;
    }
    if (bin_count_safe == 0){
      cout<<"bin_count_safe is 0"<<endl;
      bin_count_safe = 1; //ensure no division by 0
    }

    cout<<"{BGbin"<<i<<"_zjets} "<<bin_count[i]<<endl;
    cout<<"{zjets_clos_bin"<<i<<"} "<<1.+MC_Closure_Error[i]<<endl;
    cout<<"{zjets_stat_bin"<<i<<"} "<<1.+(stat_err[i]/bin_count_safe)<<endl;
//    cout<<"{zjets_stat_bin"<<i<<"} "<<1.+(noSubStatErrs[i]/bin_count_safe)<<endl;
    cout<<"{zjets_ewk_bin"<<i<<"} "<<1.+(ewk_err[i]/bin_count_safe)<<endl;
  }
  cout<<setprecision(2);

  return output_errors;
}

//event by event using variation method
pair<vector<double>,vector<double>> getFSError(const unordered_map<int,vector<double>> &bin_count, const unordered_map<int,vector<double>> &norm_up, const unordered_map<int,vector<double>> &norm_down, const unordered_map<int,vector<double>> &pt_up, const unordered_map<int,vector<double>> &pt_down, const unordered_map<int,vector<double>> &eta_up, const unordered_map<int,vector<double>> &eta_down,double Kappa, double Kappa_stat_error,TString SR){
  double kappa_MET_unc = 0.0134/0.0643 ;  //Full run 2
  double kappa_stat_unc = Kappa_stat_error/Kappa; //Crappy variable name
  vector<double> error_up;
  vector<double> error_dn;

  double bin_up, bin_dn;
  double rsfof_norm_unc_up,rsfof_norm_unc_down, rsfof_pt_unc_up,rsfof_pt_unc_down, rsfof_eta_unc_up,rsfof_eta_unc_down,bin_count_combined;

  double rsfof_norm_unc_up_years[3], rsfof_norm_unc_down_years[3],rsfof_pt_unc_up_years[3],rsfof_pt_unc_down_years[3],rsfof_eta_unc_up_years[3],rsfof_eta_unc_down_years[3];

  for(size_t i =0; i< bin_count.at(2016).size(); i++)
  {
    bin_count_combined = 0;
    for(int year = 0; year<3; year++)
    {
        bin_count_combined +=  bin_count.at(2016+year)[i];

        rsfof_norm_unc_up_years[year] = norm_up.at(2016+year)[i] - bin_count.at(2016+year)[i];
        rsfof_pt_unc_up_years[year] = pt_up.at(2016+year)[i] - bin_count.at(2016+year)[i];
        rsfof_eta_unc_up_years[year] = eta_up.at(2016+year)[i] - bin_count.at(2016+year)[i];

        rsfof_norm_unc_down_years[year] = bin_count.at(2016+year)[i]-norm_down.at(2016+year)[i];
        rsfof_pt_unc_down_years[year] = bin_count.at(2016+year)[i]-pt_down.at(2016+year)[i];
        rsfof_eta_unc_down_years[year] = bin_count.at(2016+year)[i]-eta_down.at(2016+year)[i];
    } 
    
    rsfof_norm_unc_up = 0;
    rsfof_norm_unc_down = 0;
    rsfof_pt_unc_up = 0;
    rsfof_pt_unc_down = 0;
    rsfof_eta_unc_up = 0;
    rsfof_eta_unc_down = 0;
    for(int j = 0; j<3;j++)
    {
        rsfof_norm_unc_up += rsfof_norm_unc_up_years[i] * rsfof_norm_unc_up_years[i];
        rsfof_norm_unc_down += rsfof_norm_unc_down_years[i] * rsfof_norm_unc_down_years[i];
        rsfof_pt_unc_up += rsfof_pt_unc_up_years[i] * rsfof_pt_unc_up_years[i];
        rsfof_pt_unc_down += rsfof_pt_unc_down_years[i] * rsfof_pt_unc_down_years[i];
        rsfof_eta_unc_up += rsfof_eta_unc_up_years[i] * rsfof_eta_unc_up_years[i];
        rsfof_eta_unc_down += rsfof_eta_unc_down_years[i] * rsfof_eta_unc_down_years[i];
    }
    rsfof_norm_unc_up = sqrt(rsfof_norm_unc_up);
    rsfof_norm_unc_down = sqrt(rsfof_norm_unc_down);
    rsfof_pt_unc_up = sqrt(rsfof_pt_unc_up);
    rsfof_pt_unc_down = sqrt(rsfof_pt_unc_down);
    rsfof_eta_unc_up = sqrt(rsfof_eta_unc_up);
    rsfof_eta_unc_down = sqrt(rsfof_eta_unc_down);

    RooHistError::instance().getPoissonInterval(bin_count_combined,bin_dn, bin_up);
    cout<<"bin count "<<bin_count_combined<<" Stat_Error_up "<<bin_up<<" Stat_Error_dn "<<bin_dn<<endl;

    bin_up = Kappa*Kappa*((bin_up - bin_count_combined)*(bin_up - bin_count_combined) + kappa_stat_unc*kappa_stat_unc*bin_count_combined*bin_count_combined + kappa_MET_unc * kappa_MET_unc*bin_count_combined*bin_count_combined + rsfof_norm_unc_up * rsfof_norm_unc_up + rsfof_pt_unc_up * rsfof_pt_unc_up + rsfof_eta_unc_up * rsfof_eta_unc_up);
    bin_dn = Kappa*Kappa*((bin_count_combined - bin_dn)*(bin_count_combined - bin_dn)  +  kappa_stat_unc*kappa_stat_unc*bin_count_combined*bin_count_combined + kappa_MET_unc * kappa_MET_unc*bin_count_combined*bin_count_combined + rsfof_norm_unc_down * rsfof_norm_unc_down + rsfof_pt_unc_down * rsfof_pt_unc_down + rsfof_eta_unc_down * rsfof_eta_unc_down);

    error_up.push_back(sqrt(bin_up));
    error_dn.push_back(sqrt(bin_dn));


    cout<<setprecision(10);

    if(bin_count_combined != 0)
    {
        cout<<"{rsfof_norm_unc_bin"<<i<<"} "<<1.+rsfof_norm_unc_up/bin_count_combined<<"/"<<1-rsfof_norm_unc_down/bin_count_combined<<endl;
        cout<<"{rsfof_pt_unc_bin"<<i<<"} "<<1.+rsfof_pt_unc_up/bin_count_combined<<"/"<<1-rsfof_pt_unc_down/bin_count_combined<<endl;
        cout<<"{rsfof_eta_unc_bin"<<i<<"} "<<1.+rsfof_eta_unc_up/bin_count_combined<<"/"<<1-rsfof_eta_unc_down/bin_count_combined <<endl;

    }
    else
    {
        cout<<"{rsfof_norm_unc_bin"<<i<<"} "<<1.00<<endl;
        cout<<"{rsfof_pt_unc_bin"<<i<<"} "<<1.00<<endl;
        cout<<"{rsfof_eta_unc_bin"<<i<<"} "<<1.00<<endl;
    }
    cout<<"{BGbin"<<i<<"_fsbkg} "<<bin_count_combined*Kappa<<endl;
    cout<<"{count_bin"<<i<<"_fsbkg} "<<bin_count_combined<<endl;

  }
 

  cout<<setprecision(10);
  //--------------------------------
  // To be parsed by datacard maker
  //--------------------------------
    cout<<"{kappa_MET_unc} "<<1.+kappa_MET_unc<<endl;
    cout<<"{kappa_stat_unc} "<<1.+kappa_stat_unc<<endl;

  cout<<"{rsfof*kappa} "<<Kappa<<endl; //only kappa these days

    cout<<setprecision(2);


  return make_pair(error_up, error_dn);
}



//Three years separate Rsfof and separate error
pair<vector<double>,vector<double>> getFSError(const vector<double> &bin_count,const vector <double> &bin_count_2016, const vector<double> &bin_count_2017, const vector<double> &bin_count_2018,double Kappa, double Kappa_stat_error, TString SR)
{
    double RSFOF_unc_2016 = 0.0456/1.0934;
    double RSFOF_unc_2017 = 0.0465/1.1237;
    double RSFOF_unc_2018 = 0.0447/1.0905;
    double kappa_MET_unc = 0.0134/0.0643 ;  //Full run 2
    double kappa_stat_unc = Kappa_stat_error/Kappa; //Crappy variable name

    double RSFOF_unc = 0;

    vector<double> error_up;
    vector<double> error_down;

    double bin_up, bin_dn; //statistical uncertainty for FS
     
    cout<<setprecision(10);
    for(size_t i=0; i<bin_count.size(); i++)
    {
        RooHistError::instance().getPoissonInterval(bin_count[i], bin_dn, bin_up);
        cout<<"bin count "<<bin_count[i]<<" Error_up "<<bin_up<<" Error_dn "<<bin_dn<<endl;
        bin_up = Kappa*Kappa*((bin_up - bin_count[i])*(bin_up - bin_count[i]) + RSFOF_unc_2016*RSFOF_unc_2016*bin_count_2016[i]*bin_count_2016[i] + RSFOF_unc_2017 * RSFOF_unc_2017 * bin_count_2017[i] * bin_count_2017[i] + RSFOF_unc_2018 * RSFOF_unc_2018 * bin_count_2018[i] * bin_count_2018[i] + kappa_stat_unc*kappa_stat_unc*bin_count[i]*bin_count[i] + kappa_MET_unc * kappa_MET_unc*bin_count[i]*bin_count[i] );
        
         bin_dn = Kappa*Kappa*((bin_dn - bin_count[i])*(bin_dn - bin_count[i]) + RSFOF_unc_2016*RSFOF_unc_2016*bin_count_2016[i]*bin_count_2016[i] + RSFOF_unc_2017 * RSFOF_unc_2017 * bin_count_2017[i] * bin_count_2017[i] + RSFOF_unc_2018 * RSFOF_unc_2018 * bin_count_2018[i] * bin_count_2018[i] + kappa_stat_unc*kappa_stat_unc*bin_count[i]*bin_count[i] + kappa_MET_unc * kappa_MET_unc*bin_count[i]*bin_count[i] );  
        
         //RSFOF is bin dependent now!
         RSFOF_unc = sqrt(RSFOF_unc_2016*RSFOF_unc_2016*bin_count_2016[i]*bin_count_2016[i] + RSFOF_unc_2017*RSFOF_unc_2017*bin_count_2017[i]*bin_count_2017[i] + RSFOF_unc_2018 *RSFOF_unc_2018*bin_count_2018[i]*bin_count_2018[i])/bin_count[i];
        if(bin_count[i] == 0)
        {
           RSFOF_unc = 0.0;
        }

         cout<<"{rsfof_unc_bin"<<i<<"} "<<1+RSFOF_unc<<endl;

        error_up.push_back(sqrt(bin_up));
        error_down.push_back(sqrt(bin_dn));
    }
    
    //--------------------------------
    // To be parsed by datacard maker
    //--------------------------------
    //cout<<"{rsfof_unc} "<<1.+RSFOF_unc<<endl;
    cout<<"{rsfof*kappa} "<<Kappa<<endl;
    cout<<"{kappa_MET_unc} "<<1.+kappa_MET_unc<<endl;
    cout<<"{kappa_stat_unc} "<<1.+kappa_stat_unc<<endl;

    for (int i = 0; i<(int)bin_count.size(); i++)
    {
        cout<<"{BGbin"<<i<<"_fsbkg} "<<bin_count[i]*Kappa<<endl;
        cout<<"{count_bin"<<i<<"_fsbkg} "<<bin_count[i]<<endl;
    }
    cout<<setprecision(2);
  
    return make_pair(error_up, error_down);
}


//Overall single error and single Rsfof
pair<vector<double>,vector<double>> getFSError(const vector<double> &bin_count, double RSFOFxKappa, double Kappa_stat_error, TString SR){
  double RSFOF_unc = 0.043/1.119; //Moriond 2017
  double kappa_MET_unc = 0.0134/0.0643 ;  //Full run 2
  double kappa_stat_unc = Kappa_stat_error/RSFOFxKappa; //Crappy variable name


  vector<double> error_up;
  vector<double> error_dn;

  double bin_up, bin_dn;
  for (size_t i = 0; i<bin_count.size(); i++){
    RooHistError::instance().getPoissonInterval(bin_count[i], bin_dn, bin_up);

    cout<<"bin count "<<bin_count[i]<<" Error_up "<<bin_up<<" Error_dn "<<bin_dn<<endl;
    bin_up = RSFOFxKappa*RSFOFxKappa*((bin_up - bin_count[i])*(bin_up - bin_count[i]) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i] + kappa_MET_unc*kappa_MET_unc*bin_count[i]*bin_count[i] + kappa_stat_unc * kappa_stat_unc * bin_count[i] * bin_count[i]);
    bin_dn = RSFOFxKappa*RSFOFxKappa*((bin_count[i] - bin_dn)*(bin_count[i] - bin_dn) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i] +kappa_MET_unc*kappa_MET_unc*bin_count[i]*bin_count[i] + kappa_stat_unc * kappa_stat_unc * bin_count[i] * bin_count[i] );

    error_up.push_back(sqrt(bin_up));
    error_dn.push_back(sqrt(bin_dn));
  }

  cout<<setprecision(10);
  //--------------------------------
  // To be parsed by datacard maker
  //--------------------------------
  cout<<"{rsfof_unc} "<<1.+RSFOF_unc<<endl;
  cout<<"{rsfof*kappa} "<<RSFOFxKappa<<endl;
  cout<<"{kappa_stat_unc}"<<1.+kappa_stat_unc<<endl;
  cout<<"{kappa_MET_unc}"<<1.+kappa_MET_unc<<endl;

  for (int i = 0; i<(int)bin_count.size(); i++){
    cout<<"{BGbin"<<i<<"_fsbkg} "<<bin_count[i]*RSFOFxKappa<<endl;
    cout<<"{count_bin"<<i<<"_fsbkg} "<<bin_count[i]<<endl;
  }
  cout<<setprecision(2);


  return make_pair(error_up, error_dn);
}



vector<double> getRareSamplesError(unordered_map<string,vector<double>>& all_errors, const vector<double> &bin_count, float scale, float scale_stat_unc, double scale_syst_unc, TString SR)
{
  
  //All errors map updates due to pass by reference  
  double err_bin;

  vector<double> error;

  //σ^2 = stat_err^2 + (scale*bin_count*.5)^2
  for(size_t i=0; i<bin_count.size(); i++){
    err_bin = 0;
    err_bin+= all_errors["stat"][i] * all_errors["stat"][i];
    err_bin += scale_syst_unc*scale_syst_unc*bin_count[i]*bin_count[i];

    all_errors["scale_syst"].push_back(scale_syst_unc*scale_syst_unc*bin_count[i]*bin_count[i]);

    err_bin += (scale_stat_unc/scale) * (scale_stat_unc/scale) * bin_count[i] * bin_count[i];
    
    all_errors["scale_stat"].push_back((scale_stat_unc/scale) * (scale_stat_unc/scale) * bin_count[i] * bin_count[i]);
    error.push_back(sqrt(err_bin));
  }


  /*//--------------------------------
  // To be parsed by datacard maker
  //--------------------------------

  cout<<"{BGmet100to150_mcbkg} "<<bin_count[2]<<endl;
  cout<<"{mc_stat_met100to150} "<<stat_err[2]<<endl;

  cout<<"{BGmet150to250_mcbkg} "<<bin_count[3]<<endl;
  cout<<"{mc_stat_met150to250} "<<stat_err[3]<<endl;

  if(SR == "TChiWZ"){
    cout<<"{BGmet250to350_mcbkg} "<<bin_count[4]<<endl;
    cout<<"{mc_stat_met250to350} "<<stat_err[4]<<endl;

    cout<<"{BGmet350toInf_mcbkg} "<<bin_count[5]<<endl;
    cout<<"{mc_stat_met350toInf} "<<stat_err[5]<<endl;
  }
  else{
    cout<<"{BGmet250toInf_mcbkg} "<<bin_count[4]<<endl;
    cout<<"{mc_stat_met250toInf} "<<stat_err[4]<<endl;
  }*/

  return error;
}

void printErrors(const vector<double> &temp_err, const vector<double> &rare_err, const pair<vector<double>, vector<double>> &fs_err, const vector<double> &bin_low){
  cout<<"\\MET [GeV] ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i]<<"-"<<bin_low[i+1]<<" ";
  }
  cout<<endl;
  cout<<"DY+jets ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<"+/-"<<temp_err[i]<<" ";
  }
  cout<<endl;
  cout<<"FS ";
  for (size_t i = 0; i<fs_err.first.size(); i++){
    cout<<"+"<<fs_err.first[i]<<"-"<<fs_err.second[i]<<" ";
  }
  cout<<endl;
  cout<<"Rares ";
  for (size_t i = 0; i<rare_err.size(); i++){
    cout<<"+/-"<<rare_err[i]<<" ";
  }
  cout<<endl;
  cout<<"Sum ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<"+"<<temp_err[i]+rare_err[i]+fs_err.first[i]<<"-"<<temp_err[i]+rare_err[i]+fs_err.second[i]<<" ";
  }
  cout<<endl;
}


std::pair<std::vector<double>,std::vector<double>> getTau21Error(const std::vector<double> count_central_2016,std::vector<double> count_central_2017,std::vector<double> count_central_2018,const std::vector<double> count_tau21_up_2016,std::vector<double> count_tau21_up_2017,const std::vector<double> count_tau21_up_2018,const std::vector<double> count_tau21_down_2016,const std::vector<double> count_tau21_down_2017,const std::vector<double> count_tau21_down_2018)
{
    //Also write datacard outputs here
    
    vector<double> error_2016_up,error_2016_down,error_2017_up,error_2017_down,error_2018_up,error_2018_down;

    vector<double> error_up, error_down,count_central;

    for(size_t i=0; i< count_central_2016.size(); i++)
    {
        //cout<<"tau21_up_2016="<<count_tau21_up_2016[i]<<" "<<"central_2016="<<count_central_2016[i]<<" tau21_down_2016="<<count_tau21_down_2016[i]<<endl;
        error_2016_up.push_back(count_tau21_up_2016[i] - count_central_2016[i]);
        error_2016_down.push_back(count_central_2016[i] - count_tau21_down_2016[i]);

        error_2017_up.push_back(count_tau21_up_2017[i] - count_central_2017[i]);
        error_2017_down.push_back(count_central_2017[i] - count_tau21_down_2017[i]);

        error_2018_up.push_back(count_tau21_up_2018[i] - count_central_2018[i]);
        error_2018_down.push_back(count_central_2018[i] - count_tau21_down_2018[i]);
    }

    for(size_t i = 0; i<count_central_2016.size();i++)
    {
        error_up.push_back(sqrt(error_2016_up[i] * error_2016_up[i] + error_2017_up[i] * error_2017_up[i] + error_2018_up[i] * error_2018_up[i]));
        error_down.push_back(sqrt(error_2016_down[i] * error_2016_down[i] + error_2017_down[i] * error_2017_down[i] + error_2018_down[i] * error_2018_down[i]));
        count_central.push_back(count_central_2016[i] + count_central_2017[i] + count_central_2018[i]);
    }


    return std::make_pair(error_up,error_down);
}


TGraphAsymmErrors* getErrorTGraph(const vector<double> &temp_count, const vector<double> &temp_err_up, const vector<double>& temp_err_down, const vector<double> &rare_count, const vector<double> &rare_err, const vector<double> &fs_count, const pair<vector<double>,vector<double>> &fs_err, const vector<pair<double,double>> &bin_low, const vector<double> &data_count, double RSFOF /*Really just the scale factor*/, bool ratioError, const std::vector<double>& tau21_error_up, const std::vector<double>&tau21_error_down)
{
  Double_t bin_sum[temp_err_up.size()];
  Double_t bin_err_high[temp_err_up.size()];
  Double_t bin_err_low[temp_err_up.size()];
  Double_t bin_width[temp_err_up.size()];
  Double_t bin_left[temp_err_up.size()];
  Double_t zeros[temp_err_up.size()];
  Double_t ratio[temp_err_up.size()]; //array of 1s
  pair<vector<double>,vector<double>> tau21_error = std::make_pair(tau21_error_up,tau21_error_down);
  for (size_t i = 0; i<temp_err_up.size(); i++){
    bin_sum[i] = temp_count[i]+RSFOF*fs_count[i]+rare_count[i];
    bin_err_high[i] = temp_err_up[i]*temp_err_up[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i];
    bin_err_low[i] = temp_err_down[i]*temp_err_down[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i];

    if(tau21_error.first.size() != 0 && tau21_error.second.size() != 0)
    {
       bin_err_high[i] += tau21_error.first[i] * tau21_error.first[i];
       bin_err_low[i] += tau21_error.second[i] * tau21_error.second[i];
    }

    bin_err_high[i] = sqrt(bin_err_high[i]);
    bin_err_low[i] = sqrt(bin_err_low[i]);

    if(ratioError)
    {
        bin_err_high[i] /= bin_sum[i];
        bin_err_low[i] /= bin_sum[i];
    }
    ratio[i] = 1.0;

    bin_left[i] = bin_low[i].first; //left bin so that all centers are in plot for sure.
    bin_width[i] = bin_low[i].second - bin_low[i].first; //high bin edge - low bin edge
    zeros[i] = 0; //used as x low width to get around bin center not on screen.
  }

  //TGraphAsymmErrors(num bins, x centers, y centers, x low width, x high width, y low width, y high width);
  TGraphAsymmErrors* errs;
  if(!ratioError)
  {
      errs = new TGraphAsymmErrors(temp_err_up.size(), bin_left, bin_sum, zeros, bin_width, bin_err_low, bin_err_high);
  }
  else
  {
     errs = new TGraphAsymmErrors(temp_err_up.size(), bin_left, ratio, zeros, bin_width, bin_err_low, bin_err_high);
 
  }

  return errs;

}



void printCounts(const vector<double> &temp_count, const vector<double> &temp_err, const vector<double> &rare_count, const vector<double> &rare_err, const vector<double> &fs_count, const pair<vector<double>,vector<double>> &fs_err, const vector<pair<double,double>> &bin_low, const vector<double> &data_count, double RSFOF /*Really just the scale factor*/){
  cout<<"STATTABLE: Sample ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i].first<<"-"<<bin_low[i].second<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: DY+jets ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]<<"+/-"<<temp_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: FS ";
  for (size_t i = 0; i<fs_err.first.size(); i++){
    cout<<RSFOF*fs_count[i]<<"+"<<fs_err.first[i]<<"-"<<fs_err.second[i]<<" ";
  }
  cout<<endl;
    cout<<"STATTABLE: Rares ";
  for (size_t i = 0; i<rare_err.size(); i++){
    cout<<rare_count[i]<<"+/-"<<rare_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Sum ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]+RSFOF*fs_count[i]+rare_count[i]<<"+"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i])<<"-"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i])<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Data ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<data_count[i]<<" ";
  }
  cout<<endl;
}

void printLatexCounts(const vector<double> temp_count, const vector<double> &temp_err, const vector<double> &rare_count, const vector<double> &rare_err, const vector<double> &fs_count, const pair<vector<double>,vector<double>> &fs_err, const vector<pair<double,double>> &bin_low, const vector<double> &data_count, double RSFOF /*Really just the scale factor*/){

  cout<<fixed;
  cout<<setprecision(1);

  cout<<"LATEXTABLE: \\begin{tabular} {";
  for (int i = 0; i < (int) temp_count.size(); i++){
    cout<<"l | ";
  }
  cout<<"l }"<<endl;

  cout<<"LATEXTABLE: \\MET [GeV] ";
  cout<<setprecision(0);
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<" & "<<(int) bin_low[i].first<<"-"<<(int) bin_low[i].second;
  }
  cout<<" \\\\ \\hline "<<endl;
  cout<<setprecision(1);
  cout<<"LATEXTABLE: DY+jets ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<" & "<<temp_count[i]<<"$\\pm$"<<temp_err[i];
  }
  cout<<" \\\\" <<endl;
  cout<<"LATEXTABLE: FS ";
  for (size_t i = 0; i<fs_err.first.size(); i++){
    cout<<" & "<<"$"<<RSFOF*fs_count[i]<<"^{+"<<fs_err.first[i]<<"}_{-"<<fs_err.second[i]<<"}$ ";
  }
  cout<<" \\\\" <<endl;
    cout<<"LATEXTABLE: Rares ";
  for (size_t i = 0; i<rare_err.size(); i++){
    cout<<" & "<<rare_count[i]<<"$\\pm$"<<rare_err[i];
  }
  cout<<" \\\\ \\hline" <<endl;
  cout<<"LATEXTABLE: Sum ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<" & "<<"$"<<temp_count[i]+RSFOF*fs_count[i]+rare_count[i]<<"^{+"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i])<<"}_{-"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i])<<"}$ ";
  }
  cout<<"\\\\ \\hline"<<endl;
  cout<<"LATEXTABLE: Data ";
  for (size_t i = 0; i<temp_err.size(); i++){
    cout<<" & "<<data_count[i];
  }
  cout<<endl;
  cout<<"LATEXTABLE: \\end{tabular}"<<endl;
}

void computeErrors(){
  vector<double> bin_low = {0,50,100,150,225,6001};
  vector<pair<double,double>> bin_edge;
  bin_edge.push_back(make_pair(0,50));
  bin_edge.push_back(make_pair(50,100));
  bin_edge.push_back(make_pair(100,150));
  bin_edge.push_back(make_pair(150,225));
  bin_edge.push_back(make_pair(225,6001));

  vector<double> temp_stat_err = {100,10,5,2,1};
  vector<double> temp_bin_count = {6947.05,1634.16,90.83,14.22,8};

  vector<double> FS_bin_count = {35.7,85.6,61.7,34.7,26};

  vector<double> rare_stat_err = {5,2,1,.2,.01};
  unordered_map<string,vector<double>> all_errors;
  all_errors["stat"] = rare_stat_err;
  vector<double> rare_bin_count = {12.2,18.3,9,7.9,8.9};

  vector<double> temp_err = getMetTemplatesError(temp_stat_err, temp_bin_count, sqrt(6995), 1, bin_edge, "2j");
  pair<vector<double>,vector<double>> FS_err = getFSError(FS_bin_count, 1.087,0, "2j");
  vector<double> rare_err = getRareSamplesError(all_errors, rare_bin_count, 1.5, 0.1, .5, "2j");
  cout<<"====================================\n\n\n";
  printErrors(temp_err, rare_err, FS_err, bin_low);
}
