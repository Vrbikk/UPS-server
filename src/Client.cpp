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

    char input[buffer_size];
    while(client_running){
        memset(input, 0, (size_t)buffer_size);
        int result = (int)read(connection_id, &input, (size_t)(buffer_size - 1));

        if(result <= 0){
            sending_status = false;
            Disconnection();
        }else{
            handleInput(std::string(input));
        }
    }
}

Client::Client(int connection_id, sockaddr_in address_ , Game *game_) :
        connection_id(connection_id), address(address_), game(game_) {}

void Client::initThread() {
    client_running = true;
    client_thread = std::thread(&Client::clientRunner, this);
}

Client::~Client() {
    client_running = false;

    closeConnection();

    if(client_thread.joinable()){
        client_thread.join();
    }
}

void Client::Disconnection() {
    LOGGER->Error("Client conn_id:" + std::to_string(connection_id) + " has disconnected");
    game->addIndexToGarbage(id);
    game->wakeupGarbageCollector();
    std::this_thread::sleep_for (std::chrono::milliseconds(50));
}

void Client::handleInput(std::string input) {
    if(is_valid_message(input)){
        sendMessage(compose_message(DEBUG, "ahoj"));
    }else{
        LOGGER->Error("BAD message! : " + std::string(input));
    }
}

void Client::sendMessage(message msg) {
    if(sending_status) {
        std::string a = msg.raw;
        a += '\n';
        send(connection_id, a.c_str(), a.size(), 0);
    }else{
        LOGGER->Error("Could not send message, client is not running id:" + std::to_string(id));
    }
}

void Client::closeConnection() {
    close(connection_id);
}
