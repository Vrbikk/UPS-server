//
// Created by vrbik on 19.10.16.
//

#include "tools.h"

bool is_number(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

bool file_exists(const std::string file_name){
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

void clear_string(std::string &str) {
    str.erase(std::remove(str.begin(),str.end(),' '),str.end());
}