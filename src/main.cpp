#include <iostream>
#include "entities/ship/ship.h"
#include "managers/ship_manager/ship_manager.h"
#include "entities/field/field.h"
#include <limits>
#include "./abilities/ability_manager.h"

int main() {
    ShipManager shipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    Field field(10, 10);
    AbilityManager abilityManager;

    try {
        field.placeShip(shipManager.getShip(0), 1, 1, Orientation::Horizontal);
        field.placeShip(shipManager.getShip(2), 8, 1, Orientation::Vertical);
    } catch(const ShipPlacementOutOfRange& e) {
        std::cerr << e.what() << '\n';
    }
    
    int x, y;
    bool gameOver = false;
    std::string command;

    while (!gameOver) {
        abilityManager.printAbilities();
        std::cout << "Введите команду (attack x y / ability x y): ";
        std::cin >> command;

        if (command == "attack") {
            std::cin >> x >> y;

            if (std::cin.fail()) {
                std::cerr << "Неверный ввод" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            try {
                bool hit = field.attackCell(x, y);
                if (hit) {
                    std::cout << "Сегмент уничтожен" << "\n";
                }
                field.printField();
            } catch(const AttackOutOfRangeException& e) {
                std::cerr << e.what() << '\n';
            }

        } else if (command == "ability") {
            std::cin >> x >> y;

            if (std::cin.fail()) {
                std::cerr << "Неверный ввод" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            try {
                abilityManager.applyAbility(field, x, y);
                field.printField();
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }

        } else {
            std::cerr << "Неизвестная команда" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
    }

    std::cout << "Game Over!" << std::endl;
    field.printField();
    shipManager.printShips();

    return 0;
}