#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <vector>
#include <stdexcept>
#include "../../entities/ship/ship.h"

class ShipManager {
public:
    ShipManager(const std::vector<std::pair<int, int>>& shipSizes);
    Ship& getShip(int index);
    int getShipCount() const;

private:
    std::vector<Ship> ships_;
};

#endif // SHIPMANAGER_H