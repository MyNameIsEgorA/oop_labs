#include <iostream>
#include <vector>
#include "entities/field/field.h"
#include "entities/ship/ship.h"
#include "managers/ship_manager/ship_manager.h"

int main() {
    // * - поломан, но живой; . - пусто; # - целый; ? - неизвестно
    try {
        ShipManager shipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
        GameField field(10, 10);

        field.placeShip(shipManager.getShip(0), 0, 0, Orientation::Horizontal);
        field.placeShip(shipManager.getShip(3), 6, 6, Orientation::Vertical);

        field.attackCell(0, 0, shipManager);
        field.attackCell(0, 0, shipManager);
        field.attackCell(9, 9, shipManager);
        field.attackCell(1, 0, shipManager);

        field.printField();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}