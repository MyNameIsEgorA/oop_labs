#include "./double_damage_ability.h"

void DoubleDamage::apply(Field& field, int x, int y) const {
    field.attackCell(x, y);
    field.attackCell(x, y);
}