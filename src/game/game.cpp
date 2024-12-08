#include "game.h"
#include <stdexcept>
#include <iostream>
#include "../managers/ability_manager/exceptions.h"

Game::Game()
    : userField(10, 10)
    , enemyField(10, 10)
    , userShipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE})
    , enemyShipManager({ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE})
    , currentRound(1)
    , rng(std::random_device{}()) {
    setupInitialGame();
}

void Game::setupInitialGame() {
    try {
        userField.placeShip(userShipManager.getShip(0), 1, 1, Orientation::Horizontal);
        userField.placeShip(userShipManager.getShip(1), 3, 3, Orientation::Vertical);
        userField.placeShip(userShipManager.getShip(2), 8, 1, Orientation::Vertical);
        userField.placeShip(userShipManager.getShip(3), 5, 5, Orientation::Horizontal);
        
        setupEnemyField();
    } catch(const std::exception& e) {
        throw std::runtime_error("Ошибка инициализации игры: " + std::string(e.what()));
    }
}

void Game::setupEnemyField() {
    try {
        const Ship& ship0 = enemyShipManager.getShip(0);
        const Ship& ship1 = enemyShipManager.getShip(1); 
        const Ship& ship2 = enemyShipManager.getShip(2);

        enemyField.placeShip(ship0, 1, 1, Orientation::Horizontal);
        enemyField.placeShip(ship1, 5, 5, Orientation::Horizontal);
        enemyField.placeShip(ship2, 8, 1, Orientation::Vertical);
    } catch(const std::exception& e) {
        throw std::runtime_error("Ошибка расстановки вражеских кораблей: " + std::string(e.what()));
    }
}

void Game::startNewGame() {
    std::cout << "\n=== Новая игра ===\n";
    
    userField = Field(10, 10);
    enemyField = Field(10, 10);
    userShipManager = ShipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    enemyShipManager = ShipManager({ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    abilityManager = AbilityManager();
    currentRound = 1;
    
    setupInitialGame();
    printGameState();
}

void Game::startNewRound() {
    std::cout << "\n=== Раунд " << ++currentRound << " ===\n";
    
    enemyField = Field(10, 10);
    enemyShipManager = ShipManager({ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    
    setupEnemyField();
    printGameState();
}

bool Game::makeUserTurn(int x, int y) {
    try {
        enemyField.attackCell(x, y, Attack::Default);
        if (enemyShipManager.hasDestroyedShipsChanged()) {
            this->abilityManager.addAbility();
        }
        return true;
    } catch(const AttackOutOfRangeException& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void Game::makeEnemyTurn() {
    makeRandomAttack(userField);
}

void Game::makeRandomAttack(Field& targetField) const {
    std::uniform_int_distribution<> dis(0, 9);
    
    while (true) {
        try {
            int x = dis(const_cast<std::mt19937&>(rng));
            int y = dis(const_cast<std::mt19937&>(rng));
            targetField.attackCell(x, y, Attack::Default);
            break;
        } catch(const AttackOutOfRangeException&) {
            continue;
        }
    }
}

bool Game::isAllShipsDestroyed(const ShipManager& shipManager) {
    for (int i = 0; i < shipManager.getShipCount(); ++i) {
        if (!shipManager.getShip(i).isDestroyed()) {
            return false;
        }
    }
    return true;
}

bool Game::isGameOver() const {
    return isAllShipsDestroyed(userShipManager) || isAllShipsDestroyed(enemyShipManager);
}

bool Game::hasUserWon() const {
    return isAllShipsDestroyed(enemyShipManager);
}

void Game::useAbility(int abilityIndex) {
    try {
        auto ability = abilityManager.getAbility();
        ability->apply(enemyField);
    } catch(const EmptyAbilityException& e) {
        std::cerr << e.what() << '\n';
    }
}

bool Game::canUseAbility(int abilityIndex) const {
    return this->abilityManager.isAbleToUseAbility();
}

void Game::printGameState() const {
    std::cout << "\nПоле игрока:\n";
    userField.printField();
    std::cout << "\nПоле противника:\n";
    enemyField.printField();
    std::cout << "\nСпособности:\n";
    abilityManager.printAbilities();
}