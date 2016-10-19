#include "Configuration.h"
#include "Server.h"

int main(int argc, char *argv[]) {

    LOGGER->setUp("log.log");

    if(argc == 2 && CONFIG->setUp(argv[1])){
        LOGGER->Info("Initializing servers");

            Server server(CONFIG->server_configurations[0]);
            Server serverr(CONFIG->server_configurations[1]);

        if(server.initServer() && serverr.initServer()){
            LOGGER->Info("ok");
            while(true){}
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