//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CONNECTION_H
#define SERVER2_CONNECTION_H

#include "Game.h"
#include <thread>

class Game;

class Connection {
private:
    bool accepting_running = false;
    Game *game;
    std::thread accepting_thread;

public:
    ~Connection();
    bool initConnection();
    void acceptingRunner();
    void initAccepting(Game *_game);
};

#endif //SERVER2_CONNECTION_H
