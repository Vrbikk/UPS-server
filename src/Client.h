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
    int id = -1;
    int connection_id = -1;
    std::string name = "";
    bool running = false;
public:
    std::thread client_thread;
    Client(Game *game, int id, int connection_id);
    void Update(int number);
    void sendToAll(int number);
    void runner();
    void initThread();
};

class Game;

#endif //SERVER2_CLIENT_H
