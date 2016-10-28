//
// Created by vrbik on 25.10.16.
//

#ifndef SERVER2_GAMELOGIC_H
#define SERVER2_GAMELOGIC_H

#include "Logger.h"
#include "protocol.h"
#include "Game.h"

enum game_state{
    NEW, LOGGING, GETTING_READY, PLAYING
};

class Game;

class GameLogic {
private:
    game_state main_game_state = NEW;
    Game *game = nullptr;
    std::shared_ptr<Logger> logger;

public:
    GameLogic(Game *game_, std::shared_ptr<Logger> logger_);
    void input(message msg);
};


#endif //SERVER2_GAMELOGIC_H