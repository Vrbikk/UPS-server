//
// Created by vrbik on 25.10.16.
//

#include "GameLogic.h"

GameLogic::GameLogic(Game *game_, std::shared_ptr<Logger> logger_) : game(game_), logger(logger_) {

}

void GameLogic::input(event e) {

    logger->Info("game logic input | " + game->clientInfo(e.id.index) + e.print());
    game->info();

    switch(main_game_state){
        case GETTING_READY:{
            if(e.e_type == EVENT_message && e.msg.m_type == READY_C && !game->isClientReady(e.id.index)){
                game->ready(e.id.index);
                game->sendToAllClients(compose_message(BROADCAST, game->readyList()));
            }else{
                logger->Error("bad game input");
            }

            if(game->isEveryoneReady()){
                main_game_state = PLAYING;
                game->sendToAllClients(compose_message(BROADCAST, "game started"));
            }
            break;
        }
        case PLAYING:{

            break;
        }

        default:{
            logger->Error("Unexpected main_game_state");
            break;
        }
    }

}
