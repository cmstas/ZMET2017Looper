# include "readrMuEjson.h"


//Reads JSON files provided by Marius for r_Mu/e computation
std::unordered_map<std::string,float> extractrMuEParamsFromJSON(std::string json_filename)
{
    std::string line;
    std::unordered_map<std::string,float> json_params;
    int counter;
    //First split items by comma, then split every item into kay and value by colon
    std::fstream json_file = std::fstream(json_filename,std::ios::in);
    while(std::getline(json_file,line))
    {
        line = line.substr(1,line.size()-1); //To remove the starting and trailing curly braces
        std::stringstream temp(line);
        //temp strings to hold stuff
        std::string json_temp; 
        std::string json_item;
        std::string json_key;
        std::string json_value;

        //splitting by comma
        while(std::getline(temp,json_item,','))
        {
            //json_item is one json item, now split this into key-value pair
            std::stringstream itemtemp(json_item);
            counter = 0; //to redirect split string into key/value
            while(std::getline(itemtemp,json_temp,':')) //split dict item by colon
            {
                if(counter == 0)  //key
                {
                    json_key = json_temp.substr(1,json_temp.size()-1);
                    
                    //ad-hoc string manipulation to strip spaces and double quotes from the keys
                    json_key.erase(std::remove(json_key.begin(),json_key.end(),' '),json_key.end());
                    json_key.erase(std::remove(json_key.begin(),json_key.end(),'\"'),json_key.end());
                }
                else  //value
                {
                     json_value.erase(std::remove(json_value.begin(),json_value.end(),' '),json_value.end()); 
                     json_value = json_temp;
                }
                counter ++;
            }

            //Add stuff into the unordered_map
            json_params[json_key] = stod(json_value);
        }
    }

    return json_params;
}
