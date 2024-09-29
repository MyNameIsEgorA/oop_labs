#include <iostream>
#include <vector>
#include "entities/field/field.h"
#include "managers/ship_manager/ship_manager.h"

int main() {
    std::vector<std::pair<int, int>> shipSizes = {{4, 0}, {3, 0}, {3, 0}, {2, 0}, {2, 0}, {2, 0}, {4, 0}, {3, 0}, {1, 0}, {1, 0}};
    ShipManager shipManager(shipSizes);

    GameField gameField(10, 10);

    try {
        gameField.placeShip(shipManager.getShip(7), 3, 0);
        gameField.placeShip(shipManager.getShip(8), 6, 5);
    } catch (const std::exception& e) {
        std::cerr << "Error placing ships: " << e.what() << std::endl;
    }

    gameField.printField();
    gameField.attackCell(3, 0, shipManager);
    gameField.printField();
    gameField.attackCell(3, 0, shipManager);
    gameField.printField();
    gameField.attackCell(3, 0, shipManager);
    gameField.printField();

    return 0;
}