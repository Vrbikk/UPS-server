//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CLIENT_H
#define SERVER2_CLIENT_H

#include "Game.h"
#include <iostream>
#include <thread>

class Game;

class Client {
private:
    Game *game;
    int connection_id = -1;
    std::string name = "";
    std::thread client_thread;

public:
    int id = -1;
    Client(Client const&) = delete;
    Client& operator=(Client const&) = delete;
    Client(int id, int connection_id);
    ~Client();

    bool running = false;
    void Update(int number);
    void sendToAll(int number);
    void clientRunner();
    void initThread();

};

#endif //SERVER2_CLIENT_H
