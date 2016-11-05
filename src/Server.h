//
// Created by vrbik on 19.10.16.
//

#ifndef SERVER2_SERVER_H
#define SERVER2_SERVER_H

#include <thread>
#include <memory>
#include "Game.h"
#include "Configuration.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>
#include <sys/socket.h>

class Server {
private:
    bool console_logging = false;
    int server_socket = 0;
    int tcp_queue = 10;

    bool accepting_running = false;
    std::thread accepting_thread;
    std::shared_ptr<Game> game;
    std::shared_ptr<Logger> logger;
    std::vector<question> questions;
    server_config server_conf;

public:
    bool initConnection(int port);
    void acceptingRunner();
    void initAccepting();

    Server(server_config server_config_, std::vector<question> questions_);
    ~Server();
    bool initServer();

    std::string getStatus();
};


#endif //SERVER2_SERVER_H
