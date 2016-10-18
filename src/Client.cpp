//
// Created by vrbik on 16.10.16.
//

#include "Client.h"

void Client::Update(int number) {
    std::cout << number << std::endl;
}

void Client::sendToAll(int number) {
    game->Notify(number);
}

void Client::clientRunner(){
    while(client_running){
        std::this_thread::sleep_for (std::chrono::seconds(1));
        LOGGER->Info("client:" + to_string(id) + " is running");
    }
}

Client::Client(int connection_id, Game *game_) : connection_id(connection_id), game(game_) {}

void Client::initThread() {
    client_running = true;
    client_thread = std::thread(&Client::clientRunner, this);
}

Client::~Client() {
    client_running = false;
    if(client_thread.joinable()){
        client_thread.join();
    }
}
