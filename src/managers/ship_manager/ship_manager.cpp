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

void ShipManager::printShips() const {
    int destroyedShips = 0;
    for (int i = 0; i < this->ships_.size(); i++) {
        std::cout << "Корабль " << i << ":              ";
        ships_[i].printShipInfo();
        if (ships_[i].isDestroyed()) {
            destroyedShips++;
        }
    }
    std::cout << "Итог:\n";
    std::cout << "Всего кораблей:         " << this->getShipCount() << "\n";
    std::cout << "Кораблей уничтожено:    " << destroyedShips << "\n";
    std::cout << "Кораблей не уничтожено: " << this->getShipCount() - destroyedShips << "\n";
}

