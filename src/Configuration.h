//
// Created by vrbik on 22.9.16.
//

#ifndef PLAYGROUND_CONFIGURATION_H
#define PLAYGROUND_CONFIGURATION_H

#define CONFIG Configuration::getConfiguration()

#include <string>
#include <iostream>
#include <fstream>
#include "Logger.h"
#include "tools.h"

struct server_config{
    int id = 0;
    std::string logging_file = "log.log";
    std::string name = "default";
    int port = 10000;
    int number_of_clients = 2;
};

class Configuration {

private:
    static Configuration *configuration_instance;
    Configuration();
    std::string DELIMETER = ":";
    void setBoolValue(std::string a, bool &target, std::string target_name);
    void setIntegerValue(std::string a, int &target, std::string target_name);
    bool isCommentOrEmpty(std::string line);
    void parseServerConfig(std::string line);
    std::vector<server_config> server_configurations;
    server_config default_server_configuration;

public:

    Configuration(Configuration const&) = delete;
    Configuration& operator=(Configuration const&) = delete;
    static Configuration *getConfiguration();
    void setTypeAndValue(std::string line, std::string &type, std::string &value);
    bool setUp(const std::string path);
    std::string getCurrentConfiguration();
    void destroyConfiguration();
    std::vector<server_config> getServerConfigurations();
};

#endif
