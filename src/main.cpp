#include "Configuration.h"
#include "Server.h"

int main(int argc, char *argv[]) {

    std::shared_ptr<Logger> mainLogger = std::make_shared<Logger>("log.log", true, true);
    std::shared_ptr<Configuration> config = std::make_shared<Configuration>(mainLogger);
    std::vector<std::unique_ptr<Server>> servers;

    if(argc == 2 && config->setUp(argv[1])){

        mainLogger->Info("Initializing servers");

        for(int i = 0; i < config->getServerConfigurations().size(); i++){

            server_config server_conf = config->getServerConfigurations().at(i);
            std::vector<question> questions = config->getQuestions();

            std::unique_ptr<Server> server = std::unique_ptr<Server>(new Server(server_conf, questions));
            if(server->initServer()){
                servers.push_back(std::move(server));
                mainLogger->Info(server_conf.get_server_name() + " initialized");
                mainLogger->Info("with params: " + server_conf.get_server_params());
            }else{
                mainLogger->Error(server_conf.get_server_name() + " failed during init");
            }
        }
    }

    while(true){
        std::cout << "console>> ";
        std::string input;
        std::cin >> input;

        if(!input.compare("exit")){
            mainLogger->Info("shutting down servers...");
            break;
        }
        if(!input.compare("id_")){
            for(auto&& a : servers){
                mainLogger->Info(a->getStatus());
            }
        }
    }

    mainLogger->Info("bye <3");
    return 0;
}