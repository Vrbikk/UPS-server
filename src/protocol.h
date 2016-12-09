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
    DEBUG, LOGIN_C, LOGIN_S, BROADCAST, UNICAST_S, READY_C, READY_S, QUESTIONS_S, CHOOSE_QUESTION_C, ANSWERING_QUESTION_C, ERROR
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
    std::string message_type_string[11] = {"DEBUG", "LOGIN_C", "LOGIN_S", "BROADCAST", "UNICAST_S",
                                           "READY_C", "READY_S", "QUESTIONS_S", "HOOSE_QUESTION_C",
                                           "ANSWERING_QUESTION_C", "ERROR"};
    message_type m_type = DEBUG;
    std::string data = "";
    std::string raw = "";
    int len = 0;

    std::string print(){
        return std::string("type:[" + message_type_string[m_type]  + "] data:[" + data + "]");
    }
};

struct event{

    std::string event_type_string[2] = {"EVENT_message", "EVENT_client_disconnected"};

    event_type e_type;
    client_id id;
    message msg;

    std::string print(){
        return std::string("event[" + event_type_string[e_type] + "] msg:[" + msg.print() + "]");
    }
};

message compose_message(message_type type_, int data);
message compose_message(message_type type_, std::string data_);
message decompose_message(std::string input);
event make_event(event_type e_type, message msg, client_id id);
event disconnection_event(client_id id);

bool is_valid_message(std::string input);

std::string get_message(std::string input);
bool advanced_data_validation(message_type type, std::string data);

#endif //SERVER2_PROTOCOL_H
