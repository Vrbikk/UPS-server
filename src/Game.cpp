//
// Created by vrbik on 15.10.16.
//

#include "Game.h"

void Game::Attach(std::unique_ptr<Client> client) {
    if(activeClients < maxClients){
        std::cout << "GAME: client added with id: " << client->id << std::endl;
        unsigned long index = (unsigned long) client->id;
        clientList.at(index) = std::move(client);
        clientList.at(index)->initThread();
        activeClients++;
    }else{
        std::cout << "ERROR: could not add more clients!\n";
    }
}

void Game::Detach(int client_id) {
                                                        //TODO thread cannot access this
    if(client_id < maxClients && client_id >= 0){
        clientList.at(client_id)->running = false;
        //TODO temp timeout
        std::this_thread::sleep_for(std::chrono::seconds(3));
        clientList.at(client_id) = NULL;
        activeClients--;
        std::cout << "GAME: client removed with id: " << client_id << std::endl;
    }else{
        std::cout << "ERROR: index for removing client out of size!\n";
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

void Game::initGame(Connection *connection_) {
    connection = connection_;
    connection->wakeupRunner();
}



