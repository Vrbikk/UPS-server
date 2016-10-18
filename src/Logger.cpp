//
// Created by vrbik on 24.9.16.
//

#include "Logger.h"

Logger *Logger::logger_instance = NULL;
ofstream Logger::log_stream;

Logger::Logger() {

    //default config
    logging_file = "log.log";
}

string Logger::get_current_date_time() {
    char time_format[] = "%d-%m-%Y %H:%M:%S";
    time_t time_now;
    struct tm * timeinfo;
    char buffer[80];
    time(&time_now);
    timeinfo = localtime(&time_now);
    strftime(buffer, 80, time_format, timeinfo);
    string str(buffer);
    return str;
}

void Logger::Log(string log_message) {
    string final_message = get_current_date_time() + ":  " + log_message;
    cout << final_message << endl;
    log_stream << final_message << endl;
}

Logger *Logger::getLogger() {
    if(logger_instance == NULL){
        logger_instance = new Logger();
    }

    return logger_instance;
}

void Logger::setUp(string path) {
    logging_file = path;
    log_stream.open(logging_file.c_str(), ios::out | ios::app);
}

void Logger::Info(string message) {
    Log("INFO - " + message);
}

void Logger::Error(string message) {
    Log("ERROR - " + message);
}




