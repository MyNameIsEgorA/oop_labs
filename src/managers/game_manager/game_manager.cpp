#include "game_manager.h"

#include <iostream>
#include <utility>

GameManager::GameManager(int width, int height, std::vector<ShipsInfo> userShipsInfo, std::vector<ShipsInfo> enemyShipsInfo)
    : fieldHeight(height),
      fieldWidth(width),
      userShipsInfo(std::move(userShipsInfo)),
      enemyShipsInfo(std::move(enemyShipsInfo)),
      userField(width, height),
      enemyField(width, height),
      userShipManager(initializeShipSizes(ActorType::User)),
      enemyShipManager(initializeShipSizes(ActorType::Enemy)) {
}

std::vector<ShipSize> GameManager::initializeShipSizes(ActorType actor) const {
    std::vector<ShipSize> shipSizes;
    if (actor == ActorType::User) {
        for (auto ship : this->userShipsInfo) {
            shipSizes.push_back(ship.size);
        }
    }
    if (actor == ActorType::Enemy) {
        for (auto ship : this->enemyShipsInfo) {
            shipSizes.push_back(ship.size);
        }
    }
    return shipSizes;
}