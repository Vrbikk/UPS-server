//
// Created by vrbik on 25.10.16.
//

#include "GameLogic.h"

GameLogic::GameLogic(Game *game_, std::shared_ptr<Logger> logger_) : game(game_), logger(logger_) {

}

void GameLogic::input(message msg) {

    logger->Info("INPUT- " + msg.print());


    switch(GAME_STATE){
        case NEW:{
            if(msg.type == LOGIN_C){





            }else{
                logger->Error("GAME STATE ERROR - " + msg.print());
            }

            break;
        };
        default:{
            break;
        }
    }

}
