//
// Created by vrbik on 22.9.16.
//

#include "Configuration.h"

Configuration *Configuration::configuration_instance = nullptr;

Configuration::Configuration() {

       //default configuration

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
            clear_string(line);

            if(!isCommentOrEmpty(line)){
                std::string type, value;
                setTypeAndValue(line, type, value);
                        // c++ does not support switching for string so... :(
                if(!type.compare("server")){parseServerConfig(value);}
                //else if(!type.compare("number_of_clients")){setIntegerValue(value, number_of_clients, "number_of_clients");}
                //else if(!type.compare("input_buffer_size")){setIntegerValue(value, input_buffer_size, "input_buffer_size");}
            }
        }
        return true;
    }else{
        LOGGER->Error("Config file was not found");
        return false;
    }
}

Configuration *Configuration::getConfiguration() {
    if(configuration_instance == nullptr){
        configuration_instance = new Configuration();
    }

    return configuration_instance;
}

void Configuration::setBoolValue(std::string a, bool &target, std::string target_name) {
    if(is_number(a) && stoi(a) >= 0 && stoi(a) <= 1) {
        target = (stoi(a) == 1); //TODO != 0 pro všechny > 0 bude true
    }else{
        LOGGER->Error("configuration file corrupted - " + target_name);
    }
}

void Configuration::setIntegerValue(std::string a, int &target, std::string target_name) {
    if(is_number(a) && stoi(a) > 0) {
        target = stoi(a);
    }else{
        LOGGER->Error("configuration file corrupted - " + target_name);
    }
}

std::string Configuration::getCurrentConfiguration() {
    std::string base = "Current Configuration: ";

    return base;
}


void Configuration::destroyConfiguration() {
    delete configuration_instance;
    configuration_instance = nullptr;
}

void Configuration::parseServerConfig(std::string line) { //TODO valid server config line
    std::vector<std::string> items = split(line, ",");

    server_config srv_conf_tmp;
    srv_conf_tmp.name = items[0];
    srv_conf_tmp.number_of_clients = std::stoi(items[1]);
    srv_conf_tmp.port = std::stoi(items[2]);
    srv_conf_tmp.id = (int)server_configurations.size();

    server_configurations.push_back(srv_conf_tmp);
}

