#ifndef RANDOM_ATTACK_ABILITY_H
#define RANDOM_ATTACK_ABILITY_H

#include <random>
#include "./base_ability.h"

class RandomAttack : public Ability {
public:
    void apply(Field& field) const override;
    void printAbility() const override;
    [[nodiscard]] bool needsCoordinates() const override { return false; }
};

#endif // RANDOM_ATTACK_ABILITY_H