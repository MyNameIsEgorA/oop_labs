#ifndef RANDOM_ATTACK_ABILITY_H
#define RANDOM_ATTACK_ABILITY_H

#include <random>
#include "./base_ability.h"

class RandomAttack : public Ability {
public:
    void apply(Field& field, int x, int y) const override;
    void printAbility() const override;
    bool needsCoordinates() const override { return false; } // Переопределяем метод
};

#endif // RANDOM_ATTACK_H