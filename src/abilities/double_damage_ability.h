#ifndef DOUBLE_DAMAGE_ABILITY_H
#define DOUBLE_DAMAGE_ABILITY_H

#include "./base_ability.h"

class DoubleDamage : public Ability {
public:
    void apply(Field& field, int x, int y) const override;
    void printAbility() const override;
};

#endif // DOUBLE_DAMAGE_H