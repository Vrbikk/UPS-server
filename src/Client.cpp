//
// Created by vrbik on 16.10.16.
//

#include "Client.h"

Client::Client(Game *_game) {
    this->game = _game;
}

void Client::Update(int number) {
    std::cout << number << std::endl;
}

void Client::sendToAll(int number) {
    this->game->Notify(number); //TODO PROBLEM
}
