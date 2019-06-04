#ifndef CONFIG_PARSER
#define CONFIG_PARSER

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <map>

using namespace std;

bool isWhiteSpace(const string &str);

class ConfigParser
{
    private:
	ifstream *config_file;
	string conf_path;
	map<string, string> options;
	map<string, string> default_options;
	int currentLocation=0;
    
    string cleanedArg(const string &arg);
    bool addOpt(string opt_kjey, string opt_value, bool default_opt = false);
    void extractOptFromLine(string line, bool default_opt = false);
    void loadIncludedFiles(std::string incl_fileName);

    public:
    ConfigParser(string filename);
    string findFirstConfig();
    bool hasKey(const map<string, string> &m, const string &key);
    string findNextConfig();
    bool loadNextConfig();
    bool loadConfig(string config_name);
    void print();
    string get(string key);
    string operator [] (string key);
};


#endif

