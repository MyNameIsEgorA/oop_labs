#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../../entities/field/field.h"
#include "../../entities/ship/ship.h"
#include "../ability_manager/ability_manager.h"
#include "../ship_manager/ship_manager.h"
#include <memory>

struct ShipsInfo {
    ShipSize size;
    int PlacementX;
    int PlacementY;
};

class GameManager {
public:
    GameManager();
    void startGame();

private:
    void initializeGame();
    void gameLoop();
    void handleUserInput();
    void makeEnemyTurn();
    void printGameState() const;
    bool isGameOver() const;
    bool hasUserWon() const;
    void setupNewRound();
    void setupNewGame();
    
    std::pair<int, int> getFieldSize() const;
    std::vector<ShipsInfo> getUserShipsInfo() const;
    std::vector<ShipsInfo> generateEnemyShipsInfo() const;
    bool placeShip(Field& field, const Ship& ship, int x, int y, Orientation orientation);
    
    int fieldWidth;
    int fieldHeight;
    std::unique_ptr<Field> userField;
    std::unique_ptr<Field> enemyField;
    std::unique_ptr<ShipManager> userShipManager;
    std::unique_ptr<ShipManager> enemyShipManager;
    std::unique_ptr<AbilityManager> abilityManager;
    int currentRound;
    bool isRunning;
};

#endif //GAME_MANAGER_H
