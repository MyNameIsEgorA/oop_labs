#include "game_manager.h"
#include "../ability_manager/exceptions.h"
#include <iostream>
#include <random>

GameManager::GameManager() : currentRound(1), isRunning(true) {
    initializeGame();
}

void GameManager::initializeGame() {
    auto [width, height] = getFieldSize();
    fieldWidth = width;
    fieldHeight = height;
    
    userField = std::make_unique<Field>(width, height);
    enemyField = std::make_unique<Field>(width, height);
    
    auto userShips = getUserShipsInfo();
    auto enemyShips = generateEnemyShipsInfo();
    
    std::vector<ShipSize> userShipSizes;
    for (const auto& shipInfo : userShips) {
        userShipSizes.push_back(shipInfo.size);
    }
    
    std::vector<ShipSize> enemyShipSizes;
    for (const auto& shipInfo : enemyShips) {
        enemyShipSizes.push_back(shipInfo.size);
    }
    
    userShipManager = std::make_unique<ShipManager>(userShipSizes);
    enemyShipManager = std::make_unique<ShipManager>(enemyShipSizes);
    abilityManager = std::make_unique<AbilityManager>();
    
    // Размещаем корабли игрока
    for (size_t i = 0; i < userShips.size(); ++i) {
        const auto& info = userShips[i];
        placeShip(*userField, userShipManager->getShip(i), info.PlacementX, info.PlacementY, Orientation::Horizontal);
    }
    
    // Делаем все клетки поля пользователя видимыми
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            userField->makePointVisible(x, y);
        }
    }
    
    // Размещаем корабли противника
    for (size_t i = 0; i < enemyShips.size(); ++i) {
        const auto& info = enemyShips[i];
        placeShip(*enemyField, enemyShipManager->getShip(i), info.PlacementX, info.PlacementY, Orientation::Horizontal);
    }
}

std::pair<int, int> GameManager::getFieldSize() const {
    int width, height;
    while (true) {
        std::cout << "Введите размер поля (ширина высота): ";
        if (std::cin >> width >> height && width > 0 && height > 0) {
            break;
        }
        std::cout << "Некорректный ввод. Попробуйте снова.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return {width, height};
}

std::vector<ShipsInfo> GameManager::getUserShipsInfo() const {
    std::vector<ShipsInfo> ships;
    int shipCount;
    
    std::cout << "Сколько кораблей вы хотите разместить? ";
    std::cin >> shipCount;
    
    for (int i = 0; i < shipCount; ++i) {
        ShipsInfo info;
        int size;
        std::cout << "Введите размер корабля " << i + 1 << " (1-4): ";
        std::cin >> size;
        info.size = static_cast<ShipSize>(size);
        
        std::cout << "Введите координаты размещения (x y): ";
        std::cin >> info.PlacementX >> info.PlacementY;
        
        ships.push_back(info);
    }
    
    return ships;
}

std::vector<ShipsInfo> GameManager::generateEnemyShipsInfo() const {
    std::vector<ShipsInfo> ships;
    ShipsInfo info;
    info.size = ShipSize::SMALL;  // Корабль размера 1
    info.PlacementX = 1;
    info.PlacementY = 1;
    ships.push_back(info);
    return ships;
}

void GameManager::startGame() {
    while (isRunning) {
        gameLoop();
    }
}

void GameManager::gameLoop() {
    printGameState();
    handleUserInput();
    
    if (!isRunning) return;
    
    if (!isGameOver()) {
        makeEnemyTurn();
    }
    
    if (isGameOver()) {
        if (hasUserWon()) {
            std::cout << "\n=== Вы выиграли раунд! ===\n";
            setupNewRound();
        } else {
            std::cout << "\n=== Вы проиграли! ===\n";
            setupNewGame();
        }
    }
}

void GameManager::handleUserInput() {
    std::string command;
    std::cout << "\nВведите команду (attack x y / ability / exit): ";
    std::cin >> command;
    
    if (command == "attack") {
        int x, y;
        if (std::cin >> x >> y) {
            try {
                enemyField->attackCell(x, y, Attack::Default);
                if (enemyShipManager->hasDestroyedShipsChanged()) {
                    abilityManager->addAbility();
                }
            } catch (const AttackOutOfRangeException& e) {
                std::cerr << e.what() << '\n';
            }
        }
    } else if (command == "ability") {
        try {
            auto ability = abilityManager->getAbility();
            ability->apply(*enemyField);
        } catch (const EmptyAbilityException& e) {
            std::cerr << e.what() << '\n';
        }
    } else if (command == "exit") {
        isRunning = false;
    }
}

void GameManager::makeEnemyTurn() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, fieldWidth - 1);
    
    while (true) {
        try {
            int x = dis(gen);
            int y = dis(gen);
            userField->attackCell(x, y, Attack::Default);
            break;
        } catch (const AttackOutOfRangeException&) {
            continue;
        }
    }
}

void GameManager::printGameState() const {
    std::cout << "\nРаунд " << currentRound << "\n";
    std::cout << "\nВаше поле:\n";
    userField->printField();
    std::cout << "\nП��ле противника:\n";
    enemyField->printField();
    std::cout << "\nСпособности:\n";
    abilityManager->printAbilities();
}

bool GameManager::isGameOver() const {
    bool userLost = true;
    bool enemyLost = true;
    
    for (int i = 0; i < userShipManager->getShipCount(); ++i) {
        if (!userShipManager->getShip(i).isDestroyed()) {
            userLost = false;
            break;
        }
    }
    
    for (int i = 0; i < enemyShipManager->getShipCount(); ++i) {
        if (!enemyShipManager->getShip(i).isDestroyed()) {
            enemyLost = false;
            break;
        }
    }
    
    return userLost || enemyLost;
}

bool GameManager::hasUserWon() const {
    for (int i = 0; i < enemyShipManager->getShipCount(); ++i) {
        if (!enemyShipManager->getShip(i).isDestroyed()) {
            return false;
        }
    }
    return true;
}

void GameManager::setupNewRound() {
    currentRound++;
    enemyField = std::make_unique<Field>(fieldWidth, fieldHeight);
    auto enemyShips = generateEnemyShipsInfo();
    
    std::vector<ShipSize> enemyShipSizes;
    for (const auto& shipInfo : enemyShips) {
        enemyShipSizes.push_back(shipInfo.size);
    }
    
    enemyShipManager = std::make_unique<ShipManager>(enemyShipSizes);
    
    for (size_t i = 0; i < enemyShips.size(); ++i) {
        const auto& info = enemyShips[i];
        placeShip(*enemyField, enemyShipManager->getShip(i), info.PlacementX, info.PlacementY, Orientation::Horizontal);
    }
}

void GameManager::setupNewGame() {
    currentRound = 1;
    initializeGame();
}

bool GameManager::placeShip(Field& field, const Ship& ship, int x, int y, Orientation orientation) {
    try {
        field.placeShip(ship, x, y, orientation);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка размещения корабля: " << e.what() << '\n';
        return false;
    }
}