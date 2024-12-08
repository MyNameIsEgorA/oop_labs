//
// Created by egor on 12.11.24.
//

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "../../entities/field/field.h"
#include "../../entities/ship/ship.h"
#include "../ability_manager/ability_manager.h"
#include "../ship_manager/ship_manager.h"


enum ActorType {
    User,
    Enemy
};


struct ShipsInfo {
    ShipSize size;
    int PlacementX;
    int PlacementY;
};


class GameManager {
public:
    GameManager(int width, int height, std::vector<ShipsInfo> userShipsInfo, std::vector<ShipsInfo> enemyShipsInfo);
    void initGame();
    void startGame();
    void saveGame();
    void loadGame();

private:
    void getFieldSize();
    void getShipsInfo();
    void initializeField(ActorType actor);
    std::vector<ShipSize> initializeShipSizes(ActorType actor) const;

    int fieldWidth;
    int fieldHeight;
    std::vector<ShipsInfo> userShipsInfo;
    std::vector<ShipsInfo> enemyShipsInfo;
    Field userField;
    Field enemyField;
    ShipManager userShipManager;
    ShipManager enemyShipManager;
    AbilityManager userAbilityManager;
};



#endif //GAME_MANAGER_H
