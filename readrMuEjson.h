#ifndef READRMUEJSON
#define READRMUEJSON

# include <fstream>
# include <iostream>
# include <unordered_map>
# include <vector>
# include <sstream>
# include <string>
# include <algorithm>

std::unordered_map<std::string,float> extractrMuEParamsFromJSON(std::fstream json_file);

#endif
