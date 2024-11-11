#include "./double_damage_ability.h"

#include <limits>

void DoubleDamage::apply(Field& field) const {

    int x, y;
    std::cout << "Нужны координаты для абилки\n";
    std::cin >> x >> y;

    if (std::cin.fail()) {
        std::cerr << "Неверный ввод" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    field.attackCell(x, y);
    field.attackCell(x, y);
}

void DoubleDamage::printAbility() const {
    std::cout << "Двойной урон\n";
}