//
// Created by vrbik on 25.10.16.
//

#include "GameLogic.h"

GameLogic::GameLogic(Game *game_, std::shared_ptr<Logger> logger_, std::vector<question> questions_) :
                    game(game_), logger(logger_), all_questions(questions_) {

    logger->Info("GameLogic loaded " + std::to_string(all_questions.size()) + " all_questions");
    number_of_questions = game->maxClients * 3;
    shuffleQuestions();
}

void GameLogic::input(event e) {

    logger->Info("game logic input | " + game->clientInfo(e.id.index) + e.print());
    game->info();

    switch(main_game_state){
        case GETTING_READY:{
            if(e.e_type == EVENT_message){
                if(e.msg.m_type == READY_C && !game->isClientReady(e.id.index)){
                    game->ready(e.id.index);
                    game->sendToAllClients(compose_message(BROADCAST, game->readyList()));
                }else{
                    logger->Error("bad game input");
                }
                if(game->isEveryoneReady()){
                    startGame();
                }
            }
            break;
        }
        case PLAYING:{
            if(e.e_type == EVENT_client_disconnected){
                if(game->activeClients < 2){
                    resetGameLogic();
                }
            }


            //game cycle

            break;
        }

        default:{
            logger->Error("Unexpected main_game_state");
            break;
        }
    }
}



void GameLogic::resetGameLogic() {
    main_game_state = GETTING_READY;
    shuffleQuestions();
}

void GameLogic::shuffleQuestions() {
    actual_questions = all_questions;
    std::random_shuffle(actual_questions.begin(), actual_questions.end());
    actual_questions.resize(number_of_questions);

    for(int i = 0; i < actual_questions.size(); i++){
        actual_questions.at(i).question_id = i;
    }
}

void GameLogic::startGame() {
    logger->Info("Everyone is ready, game started");
    game->sendToAllClients(compose_message(BROADCAST, "game started"));
    game->sendQuestionsToAllClients(actual_questions);
    main_game_state = PLAYING;
}
