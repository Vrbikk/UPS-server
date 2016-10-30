//
// Created by vrbik on 18.10.16.
//

#ifndef SERVER2_PROTOCOL_H
#define SERVER2_PROTOCOL_H

#include <string>
#include <vector>
#include "Logger.h"
#include "tools.h"

enum event_type{
    EVENT_message, EVENT_client_disconnected
};

enum message_type{
    DEBUG, LOGIN_C, LOGIN_S, BROADCAST, UNICAST_S, READY_C, READY_S, QUESTIONS_S, ERROR
};

struct client_id{
    bool pending = true;
    unsigned long index = 0;

    std::string print(){
        return std::string("index:" + std::to_string(index) +
                                   " pending:" + std::to_string(pending));
    }
};

struct message {
    message_type m_type = DEBUG;
    std::string data = "";
    int len = -1;
    std::string raw = "";

    std::string print(){
        return std::string("type:[" + std::to_string(m_type)  + "] data:[" + data + "]");
    }
};

struct event{
    event_type e_type;
    client_id id;
    message msg;

    std::string print(){
        return std::string("event[" + std::to_string(e_type) + "] msg:[" + msg.print() + "]");
    }
};

message compose_message(message_type type_, int data);
message compose_message(message_type type_, std::string data_);
message decompose_message(std::string input);
event make_event(event_type e_type, message msg, client_id id);
event disconnection_event();

bool is_valid_message(std::string input);

std::string get_message(std::string input);
bool advanced_data_validation(message_type type, std::string data);

#endif //SERVER2_PROTOCOL_H
