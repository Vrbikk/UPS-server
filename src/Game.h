//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H

#include "Client.h"
#include "Connection.h"
#include "Logger.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include <condition_variable>

class Client;
class Connection;

class Game {
private:
    std::vector<std::unique_ptr<Client>> clientList = std::vector<std::unique_ptr<Client>>(3);
    std::queue<int> garbageQueue;
    std::thread garbage_collector_thread;
    std::mutex mutex_garbage_collector;
    std::mutex mutex_add_index;
    std::condition_variable cv;
    bool garbage_collector_running = false;
    std::shared_ptr<Connection> connection;

public:
    Game(std::shared_ptr<Connection> connection_);

    virtual ~Game();
    int maxClients = 3;
    int activeClients = 0;
    void Attach(std::unique_ptr<Client> client);
    void Detach(int client_id);
    void Notify(int number);
    int getFreeIndex();
    void addIndexToGarbage(int index);
    void garbageCollectorThread();
    void initGarbageCollector();
    void startGame();
    void wakeupGarbageCollector();
};



#endif //SERVER2_GAME_H
