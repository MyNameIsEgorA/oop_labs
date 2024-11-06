#include <iostream>
#include "entities/ship/ship.h"
#include "managers/ship_manager/ship_manager.h"
#include "entities/field/field.h"
#include <limits>

int main() {
    ShipManager shipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    Field field(10, 10);

    try {
        field.placeShip(shipManager.getShip(0), 20, 1, Orientation::Horizontal);
        field.placeShip(shipManager.getShip(2), 8, 1, Orientation::Vertical);
    } catch(const ShipPlacementOutOfRange& e) {
        std::cerr << e.what() << '\n';
    }

    int x, y;
    bool gameOver = false;

    while (!gameOver) {
        std::cout << "Enter coordinates to attack (x y): ";
        std::cin >> x >> y;

        if (std::cin.fail()) {
            std::cerr << "Invalid input. Please enter two integers." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        try {
            bool hit = field.attackCell(x, y);
            if (hit) {
                std::cout << "Destroyed" << "\n";
            }
            field.printField();
        } catch(const AttackOutOfRangeException& e) {
            std::cerr << e.what() << '\n';
        }
    }

    std::cout << "Game Over!" << std::endl;
    field.printField();
    shipManager.printShips();

    return 0;
}