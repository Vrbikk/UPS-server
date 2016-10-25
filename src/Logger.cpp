//
// Created by vrbik on 24.9.16.
//

#include "Logger.h"

Logger::Logger(std::string logging_file_, bool console_output_, bool enable_logging_) :
        logging_file(logging_file_),
        console_output(console_output_),
        enable_logging(enable_logging_)
{
    log_stream.open(logging_file.c_str(), std::ios::out | std::ios::app);
}

std::string Logger::get_current_date_time() {
    char time_format[] = "%d-%m-%Y %H:%M:%S";
    time_t time_now;
    struct tm * timeinfo;
    char buffer[80];
    time(&time_now);
    timeinfo = localtime(&time_now);
    strftime(buffer, 80, time_format, timeinfo);
    std::string str(buffer);
    return str;
}

void Logger::Log(std::string log_message) {
    if(enable_logging) {
        std::lock_guard<std::mutex> lk(log_mutex);
        std::string final_message = get_current_date_time() + ":  " + log_message;

        if (console_output) {
            std::cout << final_message << std::endl;
        }

        log_stream << final_message << std::endl;
    }
}

void Logger::Info(std::string message) {
    Log("INFO - " + message);
}

void Logger::Error(std::string message) {
    Log("ERROR - " + message);
}

Logger::~Logger() {
    log_stream.close();
}




