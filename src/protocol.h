//
// Created by vrbik on 18.10.16.
//

#ifndef SERVER2_PROTOCOL_H
#define SERVER2_PROTOCOL_H

#include <string>

enum message_type{
    DEBUG, LOGIN, LOGIN_ANS,
};


struct message {
    message_type type;
    std::string data;
    char *raw;
};

message compose_message(message_type type_, std::string data_);
message decompose_message(char *raw);
bool is_valid_message(char *input);



#endif //SERVER2_PROTOCOL_H
