#include <iostream>
#include "entities/ship/ship.h"
#include "managers/ship_manager/ship_manager.h"
#include "entities/field/field.h"
#include <limits>
#include "./abilities/ability_manager.h"
#include "abilities/exceptions.h"

int main() {
    ShipManager shipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    Field field(10, 10);
    AbilityManager abilityManager;

    try {
        field.placeShip(shipManager.getShip(0), 1, 1, Orientation::Horizontal);
        field.placeShip(shipManager.getShip(2), 8, 1, Orientation::Vertical);
    } catch(const ShipPlacementOutOfRangeException& e) {
        std::cerr << e.what() << '\n';
    } catch(const ShipIncorrectPlacementException& e) {
        std::cerr << e.what() << '\n';
    }
    
    int x, y;
    bool gameOver = false;
    std::string command;

    while (!gameOver) {
        if (shipManager.hasDestroyedShipsChanged() == true) {
            abilityManager.addAbility();
        }
        abilityManager.printAbilities();
        std::cout << "Введите команду (attack x y / ability / Конец): ";
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

            try {
                std::shared_ptr<Ability> ability = abilityManager.getAbility();
                if (ability->needsCoordinates()) {
                    std::cout << "Нужны координаты для абилки\n";
                    std::cin >> x >> y;

                    if (std::cin.fail()) {
                        std::cerr << "Неверный ввод" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                } else {
                    std::cout << "Не нужны координаты для абилки\n";
                    x = y = -1; // Не используемые координаты
                }

                ability->apply(field, x, y);
                field.printField();
            } catch(const EmptyAbilityException& e) {
                std::cerr << e.what() << '\n';
            }

        } else if (command == "Конец") {
            gameOver = true;
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