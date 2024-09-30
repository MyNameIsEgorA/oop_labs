#include "ship_manager.h"

ShipManager::ShipManager(const std::vector<ShipSize>& shipSizes) {
    for (const auto& size : shipSizes) {
        ships_.emplace_back(size);
    }
}

Ship& ShipManager::getShip(int index) {
    if (index < 0 || index >= ships_.size()) {
        throw std::out_of_range("Invalid ship index");
    }
    return ships_[index];
}

int ShipManager::getShipCount() const {
    return ships_.size();
}

void ShipManager::hitShipSegment(Ship& ship, int segmentIndex) {
    ship.hitSegment(segmentIndex);
}