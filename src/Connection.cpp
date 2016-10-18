//
// Created by vrbik on 16.10.16.
//

#include "Connection.h"

bool Connection::initConnection() {
    return true;
}

void Connection::acceptingRunner() {
    while(accepting_running) {

        //TODO accepting

        std::this_thread::sleep_for (std::chrono::seconds(1));
        std::unique_ptr<Client> client = std::unique_ptr<Client>(new Client(0, game)); //TODO port
        game->Attach(std::move(client));
    }
}

void Connection::initAccepting(Game *_game) {
    accepting_running = true;
    game = _game;
    accepting_thread = std::thread(&Connection::acceptingRunner, this);
}

Connection::~Connection() {

    accepting_running = false;

    if(accepting_thread.joinable()){
        accepting_thread.join();
    }
}


