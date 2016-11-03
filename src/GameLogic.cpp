//
// Created by vrbik on 25.10.16.
//

#include "GameLogic.h"

GameLogic::GameLogic(Game *game_, std::shared_ptr<Logger> logger_, std::vector<question> questions_) :
                    game(game_), logger(logger_), all_questions(questions_) {

    logger->Info("GameLogic loaded " + std::to_string(all_questions.size()) + " all_questions");
    number_of_questions = game->maxClients * 3;
    shuffleQuestions();

    waiting = false;
    running_timer = true;
    waiting_thread = std::thread(&GameLogic::waitingThread, this);
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

                if(e.id.index == actual_player_index){
                    if(answering){
                        declineAnswer();
                    }else{
                        declineChoosing();
                    }
                }

                if(game->activeClients < 2){
                    logger->Error("Not enough players, back to state GETTING_READY");
                    game->sendMessageToAllClients(compose_message(ERROR, "waiting 10s for players..."));

                    waitForPlayer();

                // if disconnected player is actual player
                }
            }

            if(e.e_type == EVENT_message && game->activeClients >= 2){
                if(e.msg.m_type == READY_C && !game->isClientReady(e.id.index)){
                    runningGameReady(e.id.index);
                }
                else if(e.msg.m_type == CHOOSE_QUESTION_C && game->isClientReady(e.id.index)
                        && e.id.index == actual_player_index && !answering && isQuestionAvaible(e.msg.data)){
                    chooseQuestion(e);
                }

                else if(e.msg.m_type == ANSWERING_QUESTION_C && game->isClientReady(e.id.index) && answering
                        && actual_player_index == e.id.index){
                        answer(e);
                        nextTurn();
                }
            }
            break;
        }
        case WAITING:{
            if(e.msg.m_type == READY_C && !game->isClientReady(e.id.index) && waiting){
                runningGameReady(e.id.index);
                resumePlaying();
            }
        }
        default:{
            logger->Error("Unexpected main_game_state");
            break;
        }
    }
}

void GameLogic::resetGameLogic() {

    game->cleaningClients();
    game->sendMessageToAllClients(compose_message(BROADCAST, game->readyList()));
    game->sendMessageToAllClients(compose_message(BROADCAST, "the Game has been reseted/waiting for new players..."));

    main_game_state = GETTING_READY;
    actual_player_index = -1;
    answering = false;
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
    actual_player_index = -1;
    logger->Info("New game started");
    game->sendMessageToAllClients(compose_message(BROADCAST, "> new Game started!"));
    shuffleQuestions();
    game->sendMessageToAllClients(compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));
    setNextPlayerIndex();
    game->clearScore();
    main_game_state = PLAYING;
}

void GameLogic::setNextPlayerIndex() {
    actual_player_index = game->getNextPlayerIndex(actual_player_index);

    game->sendMessageToAllClients(compose_message(BROADCAST, "> " + game->getClientName(actual_player_index) + " is choosing question"));

    logger->Info("actual_player_index:" + std::to_string(actual_player_index));
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

void GameLogic::runningGameReady(unsigned long index) {
    game->ready(index);
    game->sendMessageToClient(index, compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));
    game->sendMessageToAllClients(
            compose_message(BROADCAST, game->getClientName(index) + " has reconnected!"));
}

void GameLogic::chooseQuestion(event e) {
    answering = true;
    answering_question = std::stoi(e.msg.data);
    game->sendMessageToAllClients(compose_message(BROADCAST, "//" + actual_questions.at(answering_question).question + "/"));
}

void GameLogic::answer(event e) {
    if(compareAnswers(e.msg.data)) {
        game->sendMessageToAllClients(compose_message(BROADCAST, game->getClientName(e.id.index)
                                                                 + " answered - " + e.msg.data));
        game->increasePoints(e.id.index, actual_questions.at(answering_question).points);
    }else{
        game->sendMessageToAllClients(compose_message(BROADCAST, game->getClientName(e.id.index)
                                                                 + " answered - " + e.msg.data));
        game->sendMessageToAllClients(compose_message(BROADCAST, "> bad answer!"));
    }

    actual_questions.at(answering_question).avaible = false;
    answering = false;
}

bool GameLogic::compareAnswers(std::string ans) {
    std::string correct_answer = actual_questions.at(answering_question).answer;

    std::transform(correct_answer.begin(), correct_answer.end(), correct_answer.begin(), ::toupper);
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);

    if(ans.compare(correct_answer) == 0){
        return true;
    }else{
        return false;
    }
}

void GameLogic::declineAnswer() {
    logger->Error("Declining answer");
    actual_questions.at(answering_question).avaible = false;
    answering = false;
    nextTurn();
}

void GameLogic::nextTurn() {
    logger->Info("Next turn");
    //updates questions for all clients
    game->sendMessageToAllClients(compose_message(QUESTIONS_S, game->getQuestionsData(actual_questions)));

    if(isMoreQuestionsAvaible()){
        setNextPlayerIndex();
    }else{
        game->gameResult();
        startNewGame();
    }
}

void GameLogic::declineChoosing() {
    logger->Error("Declining choosing");
    nextTurn();
}

void GameLogic::waitForPlayer() {
    main_game_state = WAITING;
    std::lock_guard<std::mutex> lk(mutex_waiting);
    waiting = true;
    cv.notify_one();
}

void GameLogic::waitingThread() {
    while(running_timer){
        std::unique_lock<std::mutex> lk(mutex_waiting);
        cv.wait(lk, [&]{return waiting;});

        if(running_timer){
            for(int i = 0; i < waiting_seconds * 10; i++){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                if(reconnected){
                    break;
                }

                if(interrupted){
                    break;
                }

                if(i % 10 == 0){
                    game->sendMessageToAllClients(compose_message(BROADCAST, std::to_string((i / 10) + 1) + " waiting..."));
                }
            }

            if(!reconnected || interrupted){
                logger->Info("interrupted");
                resetGameLogic();
            }

            reconnected = false;
            interrupted = false;
        }

        waiting = false;
        lk.unlock();
    }
}

void GameLogic::resumePlaying() {
    reconnected = true;
    main_game_state = PLAYING;
    game->sendMessageToAllClients(compose_message(BROADCAST, "> " + game->getClientName(actual_player_index) + " is choosing question"));
}

GameLogic::~GameLogic() {
    running_timer = false;

    {
        std::lock_guard<std::mutex> lk(mutex_waiting);
        waiting = true;
        cv.notify_one();
    }

    if(waiting_thread.joinable()){
        waiting_thread.join();
    }
}

void GameLogic::hardReset() {
    if(waiting){
        interrupted = true;
    }else{
        resetGameLogic();
    }
}
