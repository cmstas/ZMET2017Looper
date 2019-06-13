# include "doAll.h"

void runScanChain(ZMETLooper l,ConfigParser* conf){
  cout<<"Using config:"<<endl;
  conf->print();

  if (conf->get("vpt_reweight") == "true"){
    cout<<"Making Reweight Histogram"<<endl;
    if(conf->get("signal_region") == "all")
    {
      makePtReweightHisto_allSR(conf);
    }
    else
    {
        makePtReweightHisto(conf);
    }
  }

  l.ScanChain(getTChain(conf->get("data_set")), conf);
}

void doAll (ZMETLooper l,TString config_name, TString config_file) {

  ConfigParser *conf = new ConfigParser(config_file.Data());

  if (config_name == "all") {
    while ( conf->loadNextConfig() )
    {
      runScanChain(l,conf);
    }
  }

  else if ( conf->loadConfig(config_name.Data()) ){
    runScanChain(l,conf);
  }

  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  TString config_name= argv[1];
  TString config_file=argv[2];

  class ZMETLooper l;
  doAll(l,config_name, config_file);

  return 0;
}
