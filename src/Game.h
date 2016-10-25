//
// Created by vrbik on 15.10.16.
//

#ifndef SERVER2_GAME_H
#define SERVER2_GAME_H

#include "Client.h"
#include "Logger.h"
#include <thread>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
#include <mutex>
#include <condition_variable>

class Client;

class Game {
private:
    std::vector<std::unique_ptr<Client>> clientList = std::vector<std::unique_ptr<Client>>(3);
    std::queue<int> garbageQueue;
    std::thread garbage_collector_thread;
    std::mutex mutex_garbage_collector;
    std::mutex mutex_add_index;
    std::condition_variable cv;
    bool garbage_collector_running = false;
    bool garbage_ready;
    std::shared_ptr<Logger> logger;

public:
    Game(std::shared_ptr<Logger> logger_, int number_of_clients);
    virtual ~Game();
    int maxClients = 2;
    int activeClients = 0;
    void Attach(std::unique_ptr<Client> client);
    void Detach(int client_id);
    int getFreeIndex();
    void addIndexToGarbage(int index);
    void garbageCollectorThread();
    void initGarbageCollector();
    void wakeupGarbageCollector();
};



#endif //SERVER2_GAME_H
