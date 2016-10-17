//
// Created by vrbik on 16.10.16.
//

#include "Client.h"


void Client::Update(int number) {
    std::cout << number << std::endl;
}

void Client::sendToAll(int number) {
    this->game->Notify(number);
}

void Client::clientRunner(){
    while(running){
        std::this_thread::sleep_for (std::chrono::seconds(1));
        std::cout << "running:" << id << std::endl;

        game->Notify(10); //TODO PROBLEM!!!!!

    }
}

Client::Client(int id, int connection_id) : id(id), connection_id(connection_id) {
}

void Client::initThread() {
    running = true;
    client_thread = std::thread(&Client::clientRunner, this);
}

Client::~Client() {
    if(client_thread.joinable()){
        client_thread.join();
    }
}
