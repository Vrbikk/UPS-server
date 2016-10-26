//
// Created by vrbik on 25.10.16.
//

#include "GameLogic.h"

GameLogic::GameLogic(Game *game_, std::shared_ptr<Logger> logger_) : game(game_), logger(logger_) {

}

void GameLogic::input(message msg) {

    logger->Info("INPUT- " + msg.print());

    switch(main_game_state){
        case NEW:{
            if(msg.type == LOGIN_C && !game->isClientLogged(msg.client_id)){
                if(game->isUniqueLogin(msg.data)){
                    game->clientLogin(msg.client_id, msg.data);
                    game->sendToOne(msg.client_id, compose_message(LOGIN_S, std::to_string(msg.client_id)));
                }else{
                    game->sendToOne(msg.client_id, compose_message(ERROR, "nickname already used"));
                }
            }else{
                logger->Error("GAME STATE ERROR | CLIENT ALREADY LOGGED - " + msg.print());
            }

            if(game->isEveryoneLogged()){
                main_game_state = GETTING_READY;
                logger->Info("GAME STATE changed to GETTING READY");
                game->sendToAll(compose_message(BROADCAST, "Players are connected"));
                game->sendToAll(compose_message(BROADCAST, game->readyList()));
            }

            break;
        };
        case GETTING_READY:{
            if(msg.type == READY_C && !game->isClientReady(msg.client_id)){
                game->clientReady(msg.client_id);
                game->sendToAll(compose_message(BROADCAST, game->readyList()));

            }else{
                logger->Error("GAME STATE ERROR | CLIENT ALREADY READY" + msg.print());
            }

            if(game->isEveryoneReady()){
                main_game_state = PLAYING;
                game->sendToAll(compose_message(BROADCAST, "-GAME STARTED-"));
            }
            break;
        }
        case PLAYING:{

        }


        default:{
            break;
        }
    }

}
