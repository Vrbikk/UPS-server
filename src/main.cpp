#include "Configuration.h"
#include "Server.h"

int main(int argc, char *argv[]) {

    LOGGER->setUp("log.log");

    if(argc == 2 && CONFIG->setUp(argv[1])){
        LOGGER->Info("Initializing servers");
        Server server;
        if(server.initServer()){
            LOGGER->Info("ok");
            std::this_thread::sleep_for (std::chrono::seconds(10));
        }else{
            LOGGER->Error("fck");
        }
    }

    LOGGER->Info("bye :*");

    // singletons are my friends <3
    CONFIG->destroyConfiguration();
    LOGGER->destroyLogger();

    return 0;
}