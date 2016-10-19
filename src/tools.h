//
// Created by vrbik on 19.10.16.
//

#ifndef SERVER2_TOOLS_H
#define SERVER2_TOOLS_H

#include <string>
#include <algorithm>
#include <sys/stat.h>

bool is_number(const std::string &s);
bool file_exists(const std::string file_name);
void clear_string(std::string &str);

#endif //SERVER2_TOOLS_H
