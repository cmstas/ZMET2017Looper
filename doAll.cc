# include "doAll.h"

void runScanChain(ZMETLooper l,ConfigParser* conf,std::string file_name){
  cout<<"Using config:"<<endl;
  conf->print();

  if (conf->get("vpt_reweight") == "true"){
    cout<<"Making Reweight Histogram"<<endl;
    if(conf->get("signal_region") == "all")
    {
      makePtReweightHisto_allSR(conf,l.getYear());
    }
    else
    {
        makePtReweightHisto(conf,l.getYear());
    }
  }

  l.ScanChain(getTChain(conf->get("data_set"),file_name), conf);
}

void doAll (ZMETLooper l,TString config_name, TString config_file,std::string file_name) {

  ConfigParser *conf = new ConfigParser(config_file.Data());

  if (config_name == "all") {
    while ( conf->loadNextConfig() )
    {
      runScanChain(l,conf,file_name);
    }
  }

  else if ( conf->loadConfig(config_name.Data()) ){
    runScanChain(l,conf,file_name);
  }

  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {

  TString config_name,config_file;
  int year = -1;
  if(argc < 4)
  {
      cout<<"USAGE : ./ZMETLooper config_name config_file year dataset_list_file"<<endl;
      exit(1);
  }
  config_name = argv[1];
  config_file = argv[2];
  year = stoi(argv[3]);
  std::string dataset_list_file_name(argv[4]);
  class ZMETLooper l(year);
  doAll(l,config_name, config_file,dataset_list_file_name);

  return 0;
}
