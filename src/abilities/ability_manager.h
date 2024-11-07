#ifndef ABILITY_MANAGER_H
#define ABILITY_MANAGER_H

#include <vector>
#include <memory>
#include <chrono>
#include <iostream>

#include "./base_ability.h"
#include "./double_damage_ability.h"
#include "./scanner_ability.h"
#include "./random_attack_ability.h"

class AbilityManager {
public:
    AbilityManager();
    void applyAbility(Field& field, int x, int y);
    void addAbility();
    void printAbilities() const;

private:
    std::vector<std::unique_ptr<Ability>> abilities_;
};

#endif // ABILITY_MANAGER_H