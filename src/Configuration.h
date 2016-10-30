//
// Created by vrbik on 22.9.16.
//

#ifndef PLAYGROUND_CONFIGURATION_H
#define PLAYGROUND_CONFIGURATION_H

#include <string>
#include <iostream>
#include <fstream>
#include "Logger.h"
#include "tools.h"
#include <memory>

struct question{
    int question_id = 0;
    std::string question = "";
    std::string answer = "";
    int points = 0;
    bool avaible = true;
};

struct server_config{
    int id = 0;
    std::string logging_file = "log.log";
    std::string name = "default";
    int port = 10000;
    int number_of_clients = 2;
    bool enable_logging = false;

    std::string get_server_name()
    {
        std::string server_name = "Server <" + name + ">-[" + std::to_string(id) + "]";
        return server_name;
    }

    std::string get_server_params()
    {
        std::string server_params = "port: " + std::to_string(port) + ", number_of_clients: " + std::to_string(number_of_clients);
        return server_params;
    }
};

class Configuration {

private:
    std::string DELIMETER = ":";
    std::string Q_DELIMETER = "-";
    void setBoolValue(std::string a, bool &target, std::string target_name);
    void setIntegerValue(std::string a, int &target, std::string target_name);
    bool isCommentOrEmpty(std::string line);
    void parseServerConfig(std::string line);
    void parseQuestion(std::string value);

    std::vector<server_config> server_configurations;
    std::vector<question> questions;
    server_config default_server_configuration;
    std::shared_ptr<Logger> logger;


public:
    Configuration(std::shared_ptr<Logger> logger_);
    Configuration(Configuration const&) = delete;
    Configuration& operator=(Configuration const&) = delete;
    void setTypeAndValue(std::string line, std::string &type, std::string &value);
    bool setUp(const std::string path);
    std::vector<server_config> getServerConfigurations();
    std::vector<question> getQuestions();
};

#endif
