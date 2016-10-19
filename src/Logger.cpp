//
// Created by vrbik on 24.9.16.
//

#include "Logger.h"

Logger *Logger::logger_instance = nullptr;
std::ofstream Logger::log_stream;

Logger::Logger() {
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
    std::lock_guard<std::mutex> lk(log_mutex);
    std::string final_message = get_current_date_time() + ":  " + log_message;
    std::cout << final_message << std::endl;
    log_stream << final_message << std::endl;
}

Logger *Logger::getLogger() {
    if(logger_instance == NULL){
        logger_instance = new Logger();
    }

    return logger_instance;
}

void Logger::setUp(std::string path) {
    logging_file = path;
    log_stream.open(logging_file.c_str(), std::ios::out | std::ios::app);
}

void Logger::Info(std::string message) {
    Log("INFO - " + message);
}

void Logger::Error(std::string message) {
    Log("ERROR - " + message);
}




