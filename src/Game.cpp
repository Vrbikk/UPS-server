//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(Client *client) {
    clientList.push_back(client);
}

void Game::Detach(Client *client) {
    clientList.erase(std::remove(clientList.begin(), clientList.end(), client), clientList.end());
}

void Game::Notify(int number) {
    for(int i = 0; i < clientList.size(); i++){
        clientList[i]->Update(number); //TODO PROBLEM
    }
}

