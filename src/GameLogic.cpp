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

    switch(main_game_state){
        case GETTING_READY:{
            if(e.e_type == EVENT_message){
                //ready before game
                if(e.msg.m_type == READY_C && !game->isClientReady(e.id.index)){
                    game->ready(e.id.index);
                    game->sendMessageToAllClients(compose_message(BROADCAST, game->readyList()));
                }else{
                    logger->Error("bad game input - not ready while GETTING_READY");
                }
                if(game->isEveryoneReady()){
                    startNewGame();
                }
            }
            break;
        }
        case PLAYING:{
            if(e.e_type == EVENT_client_disconnected){
                //ending game while one player remains, this type of event is called every time player disconnects
                if(game->activeClients < 2){
                    logger->Error("Not enough players, back to state GETTING_READY");
                    game->sendMessageToAllClients(compose_message(ERROR, "Not enough players!"));
                    game->sendMessageToAllClients(compose_message(BROADCAST, game->readyList()));
                    resetGameLogic();
                }
            }

            if(e.e_type == EVENT_message && game->activeClients >= 2){
                //ready while running game
                if(e.msg.m_type == READY_C && !game->isClientReady(e.id.index)){
                    game->ready(e.id.index);
                    game->sendMessageToClient(e.id.index, compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));
                    game->sendMessageToAllClients(
                            compose_message(BROADCAST, game->getClientName(e.id.index) + " has reconnected!"));
                }

                //choosing questions
                else if(e.msg.m_type == CHOOSE_QUESTION_C && game->isClientReady(e.id.index)
                        && e.id.index == actual_player_index && !answering && isQuestionAvaible(e.msg.data)){
                    answering = true;
                    answering_question = std::stoi(e.msg.data);
                    answering_client_index = e.id.index;
                    game->sendMessageToAllClients(compose_message(BROADCAST, game->getClientName(e.id.index) + " question/" +
                            actual_questions.at(answering_question).question));
                }

                else if(e.msg.m_type == ANSWERING_QUESTION_C && game->isClientReady(e.id.index) && answering
                        && answering_client_index == e.id.index){

                        if(actual_questions.at(answering_question).answer.compare(e.msg.data) == 0) {
                            game->sendMessageToAllClients(compose_message(BROADCAST, game->getClientName(e.id.index)
                            + " answered - " + e.msg.data));
                            game->increasePoints(e.id.index, actual_questions.at(answering_question).points);
                        }else{
                            game->sendMessageToAllClients(compose_message(BROADCAST, game->getClientName(e.id.index)
                                                                                     + " answered - " + e.msg.data));
                            game->sendMessageToAllClients(compose_message(BROADCAST, "> Bad answer!"));
                        }

                        actual_questions.at(answering_question).avaible = false;
                        answering = false;
                        game->sendMessageToAllClients(compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));

                        if(isMoreQuestionsAvaible()){
                            setNextPlayerIndex();
                        }else{
                            game->gameResult();
                            startNewGame();
                        }
                }
            }
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
    actual_player_index = -1;
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

void GameLogic::startNewGame() {

    //TODO reset score before start

    actual_player_index = -1;
    logger->Info("New game started");
    game->sendMessageToAllClients(compose_message(BROADCAST, "> new Game started!"));
    shuffleQuestions();
    game->sendMessageToAllClients(compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));
    setNextPlayerIndex();
    main_game_state = PLAYING;
}

void GameLogic::setNextPlayerIndex() {
    actual_player_index = game->getNextPlayerIndex(actual_player_index);
}

bool GameLogic::isQuestionAvaible(std::string num) {
    int index = std::stoi(num);
    return actual_questions.at(index).avaible;
}

bool GameLogic::isMoreQuestionsAvaible() {
    for(auto question : actual_questions){
        if(question.avaible){
            return true;
        }
    }

    return false;
}
