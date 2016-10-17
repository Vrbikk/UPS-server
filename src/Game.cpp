//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(std::unique_ptr<Client> client) {
    std::cout << "GAME: client added with id: " << client->id << std::endl;
    clientList.at(client->id) = std::move(client);
    activeClients++;
}

void Game::Detach(int client_id) {
    std::cout << "GAME: client removed with id: " << client_id << std::endl;

    clientList.at(client_id)->running = false;
    //TODO temp timeout
    std::this_thread::sleep_for(std::chrono::seconds(3));
    clientList.at(client_id) = NULL;
    activeClients--;
}

void Game::Notify(int number) {
    for(int i = 0; i < clientList.size(); i++){
        clientList.at(i)->Update(number);
    }
}

void Game::initClients() {
    for(auto&& a : clientList) {
        if (!a->running) {
            a->initThread();
        }
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



