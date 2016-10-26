//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(std::unique_ptr<Client> client) {
    if(activeClients < maxClients){
        unsigned long index = (unsigned long) getFreeIndex();

        logger->Info("client added with id: " + std::to_string(index));

        client->id = (int)index;
        clientList.at(index) = std::move(client);
        clientList.at(index)->initThread();
        activeClients++;
    }else{
        logger->Error("Could not add more clients: " + std::to_string(activeClients) + "|" + std::to_string(maxClients));
        client->sendMessage(compose_message(ERROR, "Server is full"));

        std::this_thread::sleep_for (std::chrono::milliseconds(50)); // not fuckign working without this :'(

    }
}

void Game::Detach(int id) {
    if(clientExists(id)){
        clientList.at((unsigned long) id) = nullptr;
        activeClients--;
        logger->Info("REMOVING CLIENT with id: " + std::to_string(id));
    }else{
        logger->Info("could not remove client - already nullptr id:" + std::to_string(id));
    }
}

int Game::getFreeIndex() {
    int counter = 0;
    for(auto&& a: clientList){
        if(!a){
            return counter;
        }
        counter++;
    }
    return -1;
}

void Game::garbageCollectorThread() {
    while(garbage_collector_running){
        std::unique_lock<std::mutex> lk(mutex_garbage_collector);
        cv.wait(lk, [&]{return garbage_ready;});

        while(!garbageQueue.empty()) {
            Detach(garbageQueue.front());
            garbageQueue.pop();
        }

        garbage_ready = false;
        lk.unlock();
    }
}

void Game::initGarbageCollector() {
    garbage_ready = false;
    garbage_collector_running = true;
    garbage_collector_thread = std::thread(&Game::garbageCollectorThread, this);
}

void Game::wakeupGarbageCollector() {
    {
        std::lock_guard<std::mutex> lk(mutex_garbage_collector);
        garbage_ready = true;
    }
    cv.notify_one();
}

void Game::addIndexToGarbage(int index) {
    std::lock_guard<std::mutex> lk(mutex_add_index);
    garbageQueue.push(index);
}

Game::Game(std::shared_ptr<Logger> logger_, int number_of_clients_) :logger(logger_), maxClients(number_of_clients_){
    initGarbageCollector();
    gameLogic = std::unique_ptr<GameLogic>(new GameLogic(this, logger));
    clientList = std::vector<std::unique_ptr<Client>>(maxClients);
}

Game::~Game() {
    garbage_collector_running = false;
    wakeupGarbageCollector();

    if(garbage_collector_thread.joinable()){
        garbage_collector_thread.join();
    }
}

void Game::resolveMessage(message msg) {
    std::lock_guard<std::mutex> lk(mutex_input);
    gameLogic->input(msg);
}

bool Game::isUniqueLogin(std::string name) {
    for(int i = 0; i < maxClients; i++){
        if(clientList.at(i) != nullptr){
            if(clientList.at(i)->data.logged && clientList.at(i)->data.name.compare(name) == 0){
                logger->Error("same login detected");
                return false;
            }
        }
    }

    return true;
}

void Game::clientLogin(int id, std::string name) {
    if(clientExists(id)){
        clientList.at(id)->data.logged = true;
        clientList.at(id)->data.name = name;
        logger->Info("LOGGED: " + clientList.at(id)->getStatus());
    }else{
        logger->Error("could not LOGIN client - nullptr at id" + std::to_string(id));
    }
}

void Game::sendToOne(int id, message msg) {
    if(clientList.at(id) != nullptr){
        clientList.at(id)->sendMessage(msg);
    }else{
        logger->Error("could not SEND message - nullptr at id" + std::to_string(id));
    }
}

bool Game::isClientLogged(int id) {
    if(clientExists(id)){
        return clientList.at(id)->data.logged;
    }else{
        logger->Error("could not detect logged state - nullptr at id:" + std::to_string(id));
    }
}

bool Game::clientExists(int id) {
    return clientList.at(id) != nullptr;
}

std::string Game::gameStatus() {
    std::string game_status = "";

    for(auto &&a: clientList){
        if(a == nullptr){
            game_status += "----EMPTY----\n";
        }else{
            game_status += a->getStatus() + "\n";
        }
    }

    return  game_status;
}

bool Game::isEveryoneLogged() {
    for(auto &&a: clientList){
        if(a == nullptr || !a->data.logged){
            return false;
        }
    }

    return true;
}

void Game::sendToAll(message msg) {
    for(auto &&a : clientList){
        if(a != nullptr){
            a->sendMessage(msg);
        }
    }
}

std::string Game::readyList() {
    std::string list = "/---ready_list---/";

    for(auto &&a : clientList){
        if(a != nullptr){
            list += a->data.name + " - ";
            if(a->data.ready){
                list += "ready/";
            }else{
                list += "not ready/";
            }
        }
    }

    return list;
}

bool Game::isClientReady(int id) {
    if(clientExists(id)){
        return clientList.at(id)->data.ready;
    }else{
        logger->Error("could not detect ready state - nullptr at id:" + std::to_string(id));
    }
}

void Game::clientReady(int id) {
    if(clientExists(id)){
        clientList.at(id)->data.ready = true;
        logger->Info("READY: " + clientList.at(id)->getStatus());
    }else{
        logger->Error("could not READY - nullptr at id" + std::to_string(id));
    }
}

bool Game::isEveryoneReady() {
    for(auto &&a: clientList){
        if(a == nullptr || a->data.ready){
            return false;
        }
    }

    return true;
}





