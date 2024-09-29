#include "ship_manager.h"

ShipManager::ShipManager(const std::vector<std::pair<int, int>>& shipSizes) {
    for (const auto& size : shipSizes) {
        ships_.emplace_back(size.first, static_cast<Orientation>(size.second));
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