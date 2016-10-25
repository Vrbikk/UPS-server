//
// Created by vrbik on 18.10.16.
//

#ifndef SERVER2_PROTOCOL_H
#define SERVER2_PROTOCOL_H

#include <string>
#include <vector>
#include "Logger.h"
#include "tools.h"

enum message_type{
    DEBUG, LOGIN_C, LOGIN_S, BROADCAST, READY, ERROR
};

struct message {
    message_type type = DEBUG;
    std::string data = "";
    int len = -1;
    int client_id = -1;
    std::string raw = "";

    std::string print(){
        std::string text = "type: " + std::to_string(type) + " from: " + std::to_string(client_id) + " data: " + data;
        return text;
    }
};

message compose_message(message_type type_, std::string data_);
message decompose_message(std::string input, int client_id);
bool is_valid_message(std::string input);

std::string get_message(std::string input);
bool advanced_data_validation(message_type type, std::string data);

#endif //SERVER2_PROTOCOL_H
