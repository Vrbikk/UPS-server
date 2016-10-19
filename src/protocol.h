//
// Created by vrbik on 18.10.16.
//

#ifndef SERVER2_PROTOCOL_H
#define SERVER2_PROTOCOL_H

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Logger.h"

enum message_type{
    DEBUG, LOGIN, LOGIN_ANS, ERROR
};

struct char_string{
    char *content = nullptr;
    int len = 0;
};

struct message {
    message_type type = DEBUG;
    std::string data = "";
    int len = -1;
    int client_id = -1;
    string raw = "";
};

message compose_message(message_type type_, std::string data_);
message decompose_message(std::string input, int client_id);
bool is_valid_message(std::string input);

string get_message(std::string input);
std::vector<std::string> split(string message);
bool is_number(const std::string &s);
bool advanced_data_validation(message_type type, std::string data);

#endif //SERVER2_PROTOCOL_H
