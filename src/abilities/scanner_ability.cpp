#include "./scanner_ability.h"

void Scanner::apply(Field& field, int x, int y) const {
    if (bool isShipPlaced = field.checkArea(x, y)) {
        std::cout << "В заданной области находится целый корабль\n";
    } else {
        std::cout << "Данная область пуста\n";
    }
}

void Scanner::printAbility() const {
    std::cout << "Сканирование области\n";
}