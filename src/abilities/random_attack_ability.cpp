#include "./random_attack_ability.h"
#include <algorithm> // для std::shuffle

void RandomAttack::apply(Field& field) const {
    auto shipCells = field.getNonEmptyCells();

    if (shipCells.empty()) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(shipCells.begin(), shipCells.end(), gen);

    auto [randomX, randomY] = shipCells.front();

    try {
        field.hitCell(randomX, randomY);
    } catch (AttackOutOfRangeException& e) {
        std::cout << e.what() << std::endl;
    }
}

void RandomAttack::printAbility() const {
    std::cout << "Случайная атака по кораблю\n";
}