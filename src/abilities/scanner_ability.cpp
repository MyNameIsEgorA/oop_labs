#include "./scanner_ability.h"

#include <limits>

void Scanner::apply(Field& field) const {

    int x, y;
    std::cout << "Нужны координаты для абилки\n";
    std::cin >> x >> y;

    if (std::cin.fail()) {
        std::cerr << "Неверный ввод" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (field.checkArea(x, y)) {
        std::cout << "В заданной области находится целый корабль\n";
    } else {
        std::cout << "Данная область пуста\n";
    }
}

void Scanner::printAbility() const {
    std::cout << "Сканирование области\n";
}