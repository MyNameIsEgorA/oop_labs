#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <vector>
#include <stdexcept>
#include <iostream>

#include "../../entities/ship/ship.h"

class ShipManager {
public:
    explicit ShipManager(const std::vector<ShipSize>& shipSizes);

    Ship& getShip(int index);
    [[nodiscard]] int getShipCount() const;
    void printShips() const;

private:
    std::vector<Ship> ships_;
};

#endif // SHIP_MANAGER_H