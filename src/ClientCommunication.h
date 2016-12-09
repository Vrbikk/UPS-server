//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CLIENT_H
#define SERVER2_CLIENT_H

#include "Game.h"
#include <iostream>
#include <thread>
#include <netinet/in.h>
#include "protocol.h"
#include <zconf.h>
#include <sys/time.h>

class Game;

class ClientCommunication {
private:

    bool timer_running = true;
    long int last_seen = 0;
    std::shared_ptr<Game> game;
    int socket_number = -1;
    std::thread client_thread;
    std::thread timer_thread;
    sockaddr_in address;
    int buffer_size = 300;
    bool listener_running = false;
    bool sending_permission = true;
    std::shared_ptr<Logger> logger;
public:
    client_id id;
    ClientCommunication(ClientCommunication const&) = delete;
    ClientCommunication& operator=(ClientCommunication const&) = delete;

    ClientCommunication(int connection_id, sockaddr_in address_ , std::shared_ptr<Game> game_, std::shared_ptr<Logger> logger_);
    ~ClientCommunication();


    void timerRunner();
    void clientDisconnected();
    void clientRunner();
    void initThread();
    void handleInput(std::string input);
    void sendMessage(message msg);
    void closeConnection();
    std::string getStatus();

};

#endif //SERVER2_CLIENT_H
