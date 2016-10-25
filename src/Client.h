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

class Game;

struct client_data{
    std::string name = "";
};

class Client {
private:
    std::shared_ptr<Game> game;
    int connection_id = -1;
    std::thread client_thread;
    sockaddr_in address;
    int buffer_size = 300;
    bool client_running = false;
    bool sending_status = true;
    std::shared_ptr<Logger> logger;
public:

    client_data data;

    int id = -1;

    Client(Client const&) = delete;
    Client& operator=(Client const&) = delete;
    Client(int connection_id, sockaddr_in address_ , std::shared_ptr<Game> game_, std::shared_ptr<Logger> logger_);
    ~Client();

    void clientDisconnected();
    void clientRunner();
    void initThread();
    void handleInput(std::string input);
    void sendMessage(message msg);
    void closeConnection();

};

#endif //SERVER2_CLIENT_H
