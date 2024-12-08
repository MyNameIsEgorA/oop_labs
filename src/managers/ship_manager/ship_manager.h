#ifndef SHIP_MANAGER_H
#define SHIP_MANAGER_H

#include <vector>
#include <stdexcept>
#include <iostream>

#include "../../entities/ship/ship.h"

class ShipManager {
public:
    explicit ShipManager(const std::vector<ShipSize>& shipSizes);

    const Ship& getShip(int index) const;
    [[nodiscard]] int getShipCount() const;
    void printShips() const;

    bool hasDestroyedShipsChanged() const;

private:
    std::vector<Ship> ships_;
    mutable int lastDestroyedShipCount_;
};

#endif // SHIP_MANAGER_H