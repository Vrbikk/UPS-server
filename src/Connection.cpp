//
// Created by vrbik on 16.10.16.
//

#include "Connection.h"

bool Connection::initConnection() {
    return true;
}

bool ready;

void Connection::acceptingRunner() {
    while(running) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return ready;});

        while (game->activeClients < game->maxClients) {
            int index = game->getFreeIndex();

            if(index == -1){
                std::cout << "ERROR: could not add more clients!\n";
                break;
            }
            std::unique_ptr<Client> client = std::unique_ptr<Client>(new Client(index, index));
            game->Attach(std::move(client));
        }
        //game->initClients();
        ready = false;
        lk.unlock();
    }
}

void Connection::wakeupRunner() {
    ready = false;
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
    }
    cv.notify_one();
}


void Connection::setUpAccepting(Game *_game) {
    running = true;
    game = _game;
    accepting_thread = std::thread(&Connection::acceptingRunner, this);
}

Connection::~Connection() {

    running = false; //TODO ukonceni

    if(accepting_thread.joinable()){
        accepting_thread.join();
    }
}


