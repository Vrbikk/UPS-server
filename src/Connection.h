//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CONNECTION_H
#define SERVER2_CONNECTION_H

#include "Game.h"
#include "Configuration.h"
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstring>
#include <sys/socket.h>

class Game;

class Connection {
private:
    int server_socket = 0;
    int server_port = 10000;
    int tcp_queue = 10;
    bool accepting_running = false;
    std::shared_ptr<Game> game;
    std::thread accepting_thread;

public:
    ~Connection();
    bool initConnection();
    void acceptingRunner();
    void initAccepting(std::shared_ptr<Game> game_);
};

#endif //SERVER2_CONNECTION_H
