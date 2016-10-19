//
// Created by vrbik on 18.10.16.
//

#include "protocol.h"

message compose_message(message_type type_, std::string data_) {
    message msg;
    msg.type = type_;
    msg.data = data_;
    std::string msg_body = ":" + std::to_string(type_) + ":" + data_ + ";";
    int body_size = (int)msg_body.size();
    msg.len = body_size;
    msg.raw = std::to_string(body_size) + msg_body;
    return msg;
}

message decompose_message(std::string input, int client_id) {
    message msg;
    std::vector<std::string> items = split(get_message(input));

    msg.len = std::stoi(items[0]);
    msg.type = static_cast<message_type>(std::stoi(items[1]));
    msg.data = items[2];
    msg.client_id = client_id;

    return msg;
}

bool is_valid_message(std::string input) {

    if(input.empty() || (input.at(0) == '\0')){
        LOGGER->Error("not valid - null");
        return false;
    }

    std::string message = get_message(input);

    if(message.empty() || message.size() < 5){
        LOGGER->Error("not valid - bad size after cropping");
        return false;
    }

    std::vector<std::string> items = split(message);

    if(items.empty() || items.size() != 3){
        LOGGER->Error("not valid - items do not fit");
        return false;
    }

    if(!is_number(items[0]) || !is_number(items[1])){
        LOGGER->Error("not valid - type or len are not numbers");
        return false;
    }

    int len = std::stoi(items[0]);
    int type = std::stoi(items[1]);

    if((len - 1) != (items[1].size() + items[2].size() + 2)){  // 2 * DELIMETER,  without ;
        LOGGER->Error("not valid - wrong len");
        return false;
    }

    if(!(type >= DEBUG && type <= ERROR)) {
        LOGGER->Error("not valid - unknown type of message");
        return false;
    }

    if(!advanced_data_validation(static_cast<message_type>(type), items[2])){
        LOGGER->Error("not valid - advanced data validation failed");
        return false;
    }

    return true;
}

std::string get_message(std::string input) {
    return input.substr(0, input.find(';'));
}

std::vector<std::string> split(std::string message) {

    std::vector<std::string> tmp_items;

    char arr[message.size()];
    strcpy(arr, message.c_str());
    char *tokens = strtok(arr, ":");

    while(tokens != nullptr){
        tmp_items.push_back(tokens);
        tokens = strtok(nullptr, ":");
    }

    return tmp_items;
}

bool advanced_data_validation(message_type type, std::string data) {

    //TODO advanced data validation

    switch(type){
        case DEBUG:{
            return true;
        }
        case LOGIN:{
            return true;
        }
        case LOGIN_ANS:{
            return false; // cuz server -> client only
        }

        default:{
            return false;
        }
    }
}
