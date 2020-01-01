#ifndef READRMUEJSON
#define READRMUEJSON

# include <fstream>
# include <iostream>
# include <unordered_map>
# include <vector>
# include <sstream>
# include <string>
# include <algorithm>
# include <cstdlib>

std::unordered_map<std::string,float> extractrMuEParamsFromJSON(std::string json_filename);

#endif
