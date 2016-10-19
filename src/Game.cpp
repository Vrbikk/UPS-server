//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(std::unique_ptr<Client> client) {
    if(activeClients < maxClients){
        unsigned long index = (unsigned long) getFreeIndex();

        LOGGER->Info("client added with id: " + std::to_string(index));

        client->id = (int)index;
        clientList.at(index) = std::move(client);
        clientList.at(index)->initThread();
        activeClients++;
    }else{
        LOGGER->Error("could not add more clients");
        client->sendMessage(compose_message(ERROR, "Server is full"));

    }
}

void Game::Detach(int client_id) {
    if(client_id < maxClients && client_id >= 0 && clientList.at((unsigned long)client_id) != nullptr){
        clientList.at((unsigned long) client_id) = nullptr;
        activeClients--;
        LOGGER->Info("client removed with id: " + std::to_string(client_id));
    }else{
        LOGGER->Info("could not remove client - bad index: " + std::to_string(client_id));
    }
}

void Game::Notify(int number) {
    for(int i = 0; i < clientList.size(); i++){
        clientList.at(i)->Update(number);
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

void Game::startGame() {
    while(true){
        // game.Notify(200);
        std::this_thread::sleep_for (std::chrono::seconds(10));
    }
}

bool garbage_ready;

void Game::garbageCollectorThread() {
    while(garbage_collector_running){
        std::unique_lock<std::mutex> lk(mutex_garbage_collector);
        cv.wait(lk, []{return garbage_ready;});

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

Game::Game(std::shared_ptr<Connection> connection_) : connection(connection_) {
    initGarbageCollector();
}

Game::~Game() {
    garbage_collector_running = false;
    wakeupGarbageCollector();

    if(garbage_collector_thread.joinable()){
        garbage_collector_thread.join();
    }
}



