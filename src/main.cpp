#include <iostream>

#include "entities/ship/ship.h"
#include "managers/ship_manager/ship_manager.h"
#include "entities/field/field.h"

int main() {
    ShipManager shipManager({ShipSize::BIG, ShipSize::SMALL, ShipSize::MEDIUM, ShipSize::LARGE});
    // shipManager.getShip(0).getSegments()[0]->hitSegment();
    // shipManager.getShip(0).getSegments()[0]->hitSegment();
    // shipManager.getShip(1).getSegments()[0]->hitSegment();

    Field field(10, 10);
    field.placeShip(shipManager.getShip(0), 1, 1, Orientation::Horizontal);
    field.placeShip(shipManager.getShip(2), 8, 1, Orientation::Vertical);
    field.attackCell(1, 1);
    field.attackCell(1, 1);
    field.attackCell(2, 1);
    field.attackCell(9, 9);
    field.attackCell(7, 7);
    field.printField();

    shipManager.printShips();

    return 0;
}