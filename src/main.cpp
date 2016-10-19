#include "Configuration.h"

int main(int argc, char *argv[]) {

    LOGGER->setUp("log.log");

    if(argc == 2 && CONFIG->setUp(argv[1])){
        LOGGER->Info("Initializing servers");


    }else{
        LOGGER->Error("Config file not found, ending");
    }

    // singletons are my friends
    CONFIG->destroyConfiguration();
    LOGGER->destroyLogger();

    return 0;
}