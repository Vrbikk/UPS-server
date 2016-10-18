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

class Game;

class Client {
private:
    Game *game;
    int connection_id = -1;
    std::thread client_thread;
    sockaddr_in address;
    int buffer_size = 300;

public:
    std::string name = "";
    int id = -1;

    Client(Client const&) = delete;
    Client& operator=(Client const&) = delete;
    Client(int connection_id, sockaddr_in address_ , Game *game_);
    ~Client();

    bool client_running = false;
    void Update(int number);
    void sendToAll(int number);
    void clientDisconnected();
    void clientRunner();
    void initThread();
    void handleInput(char *input);

};

#endif //SERVER2_CLIENT_H
