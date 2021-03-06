//
// Created by vrbik on 22.9.16.
//

#include "Configuration.h"

Configuration::Configuration(std::shared_ptr<Logger> logger_) : logger(logger_){
       //default configuration

    default_server_configuration.logging_file = "default.log";
    default_server_configuration.port = 10000;
    default_server_configuration.id = 0;
    default_server_configuration.name = "default";
    default_server_configuration.number_of_clients = 2;
    default_server_configuration.enable_logging = false;
}

bool Configuration::isCommentOrEmpty(std::string line) {
    return line.substr(0, 1).compare("#") == 0 || line.compare("") == 0;
}

void Configuration::setTypeAndValue(std::string line, std::string &type, std::string &value) {
    type = line.substr(0, line.find(DELIMETER));
    value = line.substr(line.find(DELIMETER) + 1, line.size());
}

bool Configuration::setUp(const std::string path) {
    if(file_exists(path)){
        std::ifstream file(path);
        std::string line;

        while(getline(file, line)){


            if(!isCommentOrEmpty(line)){
                std::string type, value;
                setTypeAndValue(line, type, value);
                        // c++ does not support switching for string so... :(
                if(!type.compare("server")){
                    clear_string(value);
                    parseServerConfig(value);
                }else if(!type.compare("question")){
                    parseQuestion(value);
                }
                //else if(!m_type.compare("number_of_clients")){setIntegerValue(value, number_of_clients, "number_of_clients");}
                //else if(!m_type.compare("input_buffer_size")){setIntegerValue(value, input_buffer_size, "input_buffer_size");}
            }
        }

        if(server_configurations.empty()){
            server_configurations.push_back(default_server_configuration);
        }

        return true;
    }else{
        logger->Error("Config file was not found");
        return false;
    }
}

void Configuration::setBoolValue(std::string a, bool &target, std::string target_name) {
    if(is_number(a) && stoi(a) >= 0 && stoi(a) <= 1) {
        target = (stoi(a) == 1); //TODO != 0 pro všechny > 0 bude true
    }else{
        logger->Error("configuration file corrupted - " + target_name);
    }
}

void Configuration::setIntegerValue(std::string a, int &target, std::string target_name) {
    if(is_number(a) && stoi(a) > 0) {
        target = stoi(a);
    }else{
        logger->Error("configuration file corrupted - " + target_name);
    }
}

void Configuration::parseServerConfig(std::string line) {
    std::vector<std::string> items = split(line, ",");

    if(items.size() == 5) {
        server_config srv_conf_tmp;
        srv_conf_tmp.name = items[0];
        srv_conf_tmp.number_of_clients = std::stoi(items[1]);

        if(srv_conf_tmp.number_of_clients < 2){
            logger->Error("Number of players must be at least 2!");
            srv_conf_tmp.number_of_clients = 2;
        }

        srv_conf_tmp.port = std::stoi(items[2]);
        srv_conf_tmp.logging_file = items[3];
        srv_conf_tmp.id = (int) server_configurations.size();
        srv_conf_tmp.enable_logging = (bool)std::stoi(items[4]);
        server_configurations.push_back(srv_conf_tmp);
    }else{
        logger->Error("configuration file corrupted at line: " + line);
    }
}

std::vector<server_config> Configuration::getServerConfigurations() {
    return server_configurations;
}

void Configuration::parseQuestion(std::string value) {
    std::vector<std::string> q = split(value, Q_DELIMETER);

    if(q.size() == 4){
        question tmp_q;
        tmp_q.category = q[0];
        tmp_q.question = q[1];
        tmp_q.answer = q[2];
        tmp_q.points = std::stoi(q[3]);

        questions.push_back(tmp_q);
    }
}

std::vector<question> Configuration::getQuestions() {
    return questions;
}




