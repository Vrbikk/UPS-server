//
// Created by vrbik on 19.10.16.
//

#ifndef SERVER2_SERVER_H
#define SERVER2_SERVER_H

#include <thread>

class Server {
private:
    std::thread server_thread;
public:
    Server();
    ~Server();
    void initServer();
};


#endif //SERVER2_SERVER_H
