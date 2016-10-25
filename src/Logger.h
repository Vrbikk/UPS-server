//
// Created by vrbik on 24.9.16.
//

#ifndef PLAYGROUND_LOGGER_H
#define PLAYGROUND_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

class Logger {
private:
    std::ofstream log_stream;
    void Log(std::string log_message);
    std::string get_current_date_time();
    std::mutex log_mutex;
    std::string logging_file;
    bool console_output;
    bool enable_logging;
public:
    Logger(std::string logging_file_, bool console_output_, bool enable_logging_);
    ~Logger();

    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;
    void Info(std::string message);
    void Error(std::string message);
};

#endif
