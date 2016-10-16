//
// Created by vrbik on 16.10.16.
//

#ifndef SERVER2_CLIENT_H
#define SERVER2_CLIENT_H

#include <iostream>

class Game;

class Client {
    Game *game;
public:
    Client(Game *_game);
    void Update(int number);
    void sendToAll(int number);
};

#endif //SERVER2_CLIENT_H
