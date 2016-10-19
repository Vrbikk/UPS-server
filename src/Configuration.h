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

class Configuration {
private:
    static Configuration *configuration_instance;
    Configuration();
    std::string DELIMETER = ":";
    int port;
    int number_of_clients;
    int input_buffer_size;
    void setBoolValue(std::string a, bool &target, std::string target_name);
    void setIntegerValue(std::string a, int &target, std::string target_name);

public:
    static Configuration *getConfiguration();
    bool isCommentOrEmpty(std::string line);
    void setTypeAndValue(std::string line, std::string &type, std::string &value);
    bool setUp(const std::string path);
    int getPort();
    int getNumberOfClients();
    int getIntpuBufferSize();
    std::string getCurrentConfiguration();
};

#endif
