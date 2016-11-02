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
    GETTING_READY, PLAYING, WAITING
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
    void startNewGame();
    void setNextPlayerIndex();
    bool isQuestionAvaible(std::string num);
    bool isMoreQuestionsAvaible();
    void runningGameReady(unsigned long index);
    void chooseQuestion(event e);
    void answer(event e);
    bool compareAnswers(std::string ans);
    void declineAnswer();
    void declineChoosing();
    void nextTurn();

    int actual_player_index = -1;
    bool answering = false;
    int answering_question = 0;

    void waitForPlayer();
    void resumePlaying();

    std::thread waiting_thread;
    bool waiting;
    bool running_timer;
    std::mutex mutex_waiting;
    std::condition_variable cv;
    int waiting_seconds = 10;
    void waitingThread();

    bool reconnected = false;
    bool interrupted = false;

public:
    GameLogic(Game *game_, std::shared_ptr<Logger> logger_, std::vector<question> questions_);
    virtual ~GameLogic();

    void hardReset();
    void input(event e);
};


#endif //SERVER2_GAMELOGIC_H