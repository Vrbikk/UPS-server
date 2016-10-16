//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(Client *client) {
    clientList.push_back(client);
    clientList.back()->initThread();
    activeClients++;
    std::cout << "GAME: client added" << std::endl;
}

void Game::Detach(Client *client) {
    clientList.erase(std::remove(clientList.begin(), clientList.end(), client), clientList.end());
    activeClients--;
    std::cout << "GAME: client deleted" << std::endl;
}

void Game::Notify(int number) {
    for(int i = 0; i < clientList.size(); i++){
        clientList[i]->Update(number);
    }
}

void Game::joinClients() {
    for (auto i : clientList)
        if(i->client_thread.joinable()){
            i->client_thread.join();
        }
}



