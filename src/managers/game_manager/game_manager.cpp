#include "game_manager.h"
#include "../ability_manager/exceptions.h"
#include "../../game/game_state.h"
#include <iostream>
#include <random>
#include <fstream>

GameManager::GameManager() : isRunning(true) {
    // Пустой конструктор, инициализация будет происходить в startGame
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
    currentRound = 1;
    
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
    std::cout << "1. Загрузить сохранение\n";
    std::cout << "2. Начать новую игру\n";
    std::cout << "Выберите действие (1/2): ";
    
    int choice;
    while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cout << "Некорректный ввод. Пожалуйста, введите 1 или 2: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    if (choice == 1) {
        try {
            loadGame("save");
            std::cout << "Игра загружена\n";
        } catch (const std::exception& e) {
            std::cout << "Ошибка загрузки: " << e.what() << "\nНачинаем новую игру\n";
            initializeGame();
        }
    } else {
        initializeGame();
    }
    
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
    std::cout << "\nВведите команду (attack x y / ability / save / exit): ";
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
    } else if (command == "save") {
        saveGame("save");
        std::cout << "Игра сохранена\n";
    } else if (command == "exit") {
        saveGame("save");
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
    std::cout << "\nПоле противника:\n";
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

void GameManager::saveGame(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для сохранения");
    }
    
    std::vector<ShipSize> userShipSizes;
    std::vector<ShipSize> enemyShipSizes;
    
    for (int i = 0; i < userShipManager->getShipCount(); ++i) {
        userShipSizes.push_back(userShipManager->getShip(i).getLength());
    }
    
    for (int i = 0; i < enemyShipManager->getShipCount(); ++i) {
        enemyShipSizes.push_back(enemyShipManager->getShip(i).getLength());
    }
    
    GameState state(fieldWidth, fieldHeight, currentRound, 
                   *userField, *enemyField, 
                   userShipSizes, enemyShipSizes);
    
    file << state;
}

void GameManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для загрузки");
    }
    
    GameState state;
    file >> state;
    
    fieldWidth = state.fieldWidth;
    fieldHeight = state.fieldHeight;
    currentRound = state.currentRound;
    
    userField = std::make_unique<Field>(fieldWidth, fieldHeight);
    enemyField = std::make_unique<Field>(fieldWidth, fieldHeight);
    
    // Создаем менеджеры кораблей
    userShipManager = std::make_unique<ShipManager>(state.userShipSizes);
    enemyShipManager = std::make_unique<ShipManager>(state.enemyShipSizes);
    abilityManager = std::make_unique<AbilityManager>();
    
    // Размещаем корабли пользователя и восстанавливаем их состояние
    int currentX = 1;
    for (size_t i = 0; i < state.userShipSizes.size(); ++i) {
        placeShip(*userField, userShipManager->getShip(i), currentX, 1, Orientation::Horizontal);
        auto segments = userShipManager->getShip(i).getSegments();
        for (size_t j = 0; j < segments.size(); ++j) {
            segments[j]->setState(state.userFieldState[1][currentX + j]);
        }
        currentX += static_cast<int>(state.userShipSizes[i]) + 1;
    }
    
    // Размещаем корабль противника
    if (!state.enemyShipSizes.empty()) {
        placeShip(*enemyField, enemyShipManager->getShip(0), 1, 1, Orientation::Horizontal);
        auto segments = enemyShipManager->getShip(0).getSegments();
        for (size_t j = 0; j < segments.size(); ++j) {
            segments[j]->setState(state.enemyFieldState[1][1 + j]);
        }
    }
    
    // Восстанавливаем состояние видимости полей
    for (int y = 0; y < fieldHeight; ++y) {
        for (int x = 0; x < fieldWidth; ++x) {
            if (state.userFieldVisible[y][x]) {
                userField->makePointVisible(x, y);
            }
            if (state.enemyFieldVisible[y][x]) {
                enemyField->makePointVisible(x, y);
            }
        }
    }
    
    // Делаем все клетки поля пользователя видимыми
    for (int y = 0; y < fieldHeight; ++y) {
        for (int x = 0; x < fieldWidth; ++x) {
            userField->makePointVisible(x, y);
        }
    }
}