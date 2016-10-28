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
#include "GameLogic.h"

struct clientGameData{
    int score = 0;
    bool online = false;
};

class Client;
class GameLogic;

class Game {
private:
    std::unique_ptr<GameLogic> gameLogic;
    std::vector<std::unique_ptr<Client>> clientList;



    std::queue<int> garbageQueue;
    std::thread garbage_collector_thread;
    std::mutex mutex_garbage_collector;
    std::mutex mutex_add_index;
    std::mutex mutex_input;
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
    void Detach(int id);
    int getFreeIndex();
    void addIndexToGarbage(int index);
    void garbageCollectorThread();
    void initGarbageCollector();
    void wakeupGarbageCollector();

    void resolveMessage(message msg);
    std::string gameStatus();


    //Filip said that this is pretty nice coding style and I trust him cuz he is my friend <3
    bool clientExists(int id);
    bool isClientLogged(int id);
    bool isUniqueLogin(std::string name);
    void clientLogin(int id, std::string name);
    bool isEveryoneLogged();
    bool isClientReady(int id);
    void clientReady(int id);
    bool isEveryoneReady();
    std::string readyList();


    void sendToOne(int id, message msg);
    void sendToAll(message msg);

};



#endif //SERVER2_GAME_H
