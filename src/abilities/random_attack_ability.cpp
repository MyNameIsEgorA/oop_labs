#include "./random_attack_ability.h"

void RandomAttack::apply(Field& field, int x, int y) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(0, field.getHeight() - 1);

    int randomX = disX(gen);
    int randomY = disY(gen);

    field.attackCell(randomX, randomY);
}