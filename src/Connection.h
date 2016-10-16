//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CONNECTION_H
#define SERVER2_CONNECTION_H

#include "Game.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class Game;

class Connection {
private:
    Game *game;
public:
    std::mutex m;
    std::condition_variable cv;

    bool initConnection();
    void clientAccepting_Thread();
    void setUpAccepting(Game *_game);

};


#endif //SERVER2_CONNECTION_H
