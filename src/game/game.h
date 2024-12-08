#pragma once

#include "../managers/ship_manager/ship_manager.h"
#include "../managers/ability_manager/ability_manager.h"
#include "../entities/field/field.h"
#include <random>

class Game {
private:
    Field userField;
    Field enemyField;
    ShipManager userShipManager;
    ShipManager enemyShipManager;
    AbilityManager abilityManager;
    int currentRound;
    std::mt19937 rng;

    void setupInitialGame();
    void setupEnemyField();
    void makeRandomAttack(Field& targetField) const;
    static bool isAllShipsDestroyed(const ShipManager& shipManager) ;
    
public:
    Game();
    
    void startNewGame();
    void startNewRound();
    
    bool makeUserTurn(int x, int y);
    void makeEnemyTurn();
    
    bool isGameOver() const;
    bool hasUserWon() const;
    
    void useAbility();
    bool canUseAbility(int abilityIndex) const;
    
    void printGameState() const;
    const Field& getUserField() const { return userField; }
    const Field& getEnemyField() const { return enemyField; }
    int getCurrentRound() const { return currentRound; }
};