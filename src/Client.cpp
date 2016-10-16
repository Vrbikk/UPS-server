//
// Created by vrbik on 16.10.16.
//

#include "Client.h"


void Client::Update(int number) {
    std::cout << number << std::endl;
}

void Client::sendToAll(int number) {
    this->game->Notify(number); //TODO PROBLEM
}

void Client::runner() {
    //std::cout << "running from client:" << id << std::endl;
    while(running){
        std::this_thread::sleep_for (std::chrono::seconds(3));
        std::cout << "running from client:" << id << std::endl;
    }
}

Client::Client(Game *game, int id, int connection_id) : game(game), id(id), connection_id(connection_id) {

}

void Client::initThread() {
    running = true;
    client_thread = std::thread(&Client::runner, this);
}
