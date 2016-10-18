//
// Created by vrbik on 24.9.16.
//

#ifndef PLAYGROUND_LOGGER_H
#define PLAYGROUND_LOGGER_H
#define LOGGER Logger::getLogger()

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

using namespace std;

class Logger {
private:
    static Logger *logger_instance;
    static ofstream log_stream;
    void Log(string log_message);
    string logging_file = "log.log";
    mutex log_mutex;
    Logger();

public:
    void setUp(string path);
    void Info(string message);
    void Error(string message);
    static Logger *getLogger();
    string get_current_date_time();
};

#endif //PLAYGROUND_LOGGER_H
