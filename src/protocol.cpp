//
// Created by vrbik on 18.10.16.
//

#include "protocol.h"

message compose_message(message_type type_, std::string data_) {
    message msg;
    msg.m_type = type_;
    msg.data = data_;
    msg.raw = std::to_string(type_) + ":" + data_ + ";";

    return msg;
}

message compose_message(message_type type_, int data){
    return compose_message(type_, std::to_string(data));
}

message decompose_message(std::string input) {
    message msg;
    std::vector<std::string> items = split(get_message(input), ":");
    msg.m_type = static_cast<message_type>(std::stoi(items[0]));
    msg.data = items[1];

    return msg;
}

event make_event(event_type e_type, message msg, client_id id){
    event e;
    e.e_type = e_type;
    e.msg = msg;
    e.id = id;

    return e;
}

event disconnection_event(client_id id) {
    event e;
    e.e_type = EVENT_client_disconnected;
    e.id = id;
    return e;
}

bool is_valid_message(std::string input) {

    if(input.empty() || (input.at(0) == '\0') || input.size() > 300){
       // LOGGER->Error("not valid - null");
        return false;
    }

    std::string message = get_message(input);

    if(message.empty() || message.size() < 3){
       // LOGGER->Error("not valid - bad size after cropping");
        return false;
    }

    std::vector<std::string> items = split(message, ":");

    if(items.empty() || items.size() != 2){
      //  LOGGER->Error("not valid - items do not fit"); and if i fit i sit
        return false;
    }

    if(!is_number(items[0])){
      //  LOGGER->Error("not valid - m_type or len are not numbers");
        return false;
    }

    int type = std::stoi(items[0]);

    if(!(type >= DEBUG && type <= ERROR)) {
      //  LOGGER->Error("not valid - unknown m_type of message");
        return false;
    }

    if(!advanced_data_validation(static_cast<message_type>(type), items[1])){
      //  LOGGER->Error("not valid - advanced data validation failed");
        return false;
    }

    return true;
}

std::string get_message(std::string input) {
    return input.substr(0, input.find(';'));
}

bool advanced_data_validation(message_type type, std::string data) {

    switch(type){
        case DEBUG:{
            return true;
        }
        case LOGIN_C:{
            if(data.length() >= 2){
                return true;
            }else{
                return false;
            }
        }
        case READY_C:{
            if(data.compare("ready") == 0){
                return true;
            }else{
                return false;
            }
        }
        case CHOOSE_QUESTION_C:{
            if(is_number(data) && std::stoi(data) >= 0 && std::stoi(data) < 30){
                return true;
            }else{
                return false;
            }
        }
        case ANSWERING_QUESTION_C:{
            if(data.size() < 40){
                return true;
            }
        }
        case ERROR:{
            return true;
        }
        default:{
            return false;
        }
    }
}
