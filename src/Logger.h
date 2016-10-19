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

class Logger {
private:
    static Logger *logger_instance;
    static std::ofstream log_stream;
    void Log(std::string log_message);
    std::string logging_file = "log.log";
    std::mutex log_mutex;
    Logger();

public:
    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;

    void setUp(std::string path);
    void Info(std::string message);
    void Error(std::string message);
    static Logger *getLogger();
    std::string get_current_date_time();
    void destroyLogger();
};

#endif
