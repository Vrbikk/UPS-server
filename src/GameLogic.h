//
// Created by vrbik on 25.10.16.
//

#ifndef SERVER2_GAMELOGIC_H
#define SERVER2_GAMELOGIC_H

#include "Logger.h"
#include "protocol.h"
#include "Game.h"
#include "Configuration.h"
#include <algorithm>

enum game_state{
    GETTING_READY, PLAYING
};

class Game;

class GameLogic {
private:
    std::vector<question> all_questions;
    std::vector<question> actual_questions;
    int number_of_questions = 0;

    game_state main_game_state = GETTING_READY;
    Game *game = nullptr;
    std::shared_ptr<Logger> logger;
    void resetGameLogic();
    void shuffleQuestions();
    void startGame();
public:
    GameLogic(Game *game_, std::shared_ptr<Logger> logger_, std::vector<question> questions_);
    void input(event e);
};


#endif //SERVER2_GAMELOGIC_H