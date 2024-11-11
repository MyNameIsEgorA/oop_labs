#ifndef ABILITY_MANAGER_H
#define ABILITY_MANAGER_H

#include <vector>
#include <memory>
#include <chrono>
#include <iostream>

#include "../../abilities/base_ability.h"
#include "../../abilities/double_damage_ability.h"
#include "../../abilities/random_attack_ability.h"
#include "../../abilities/scanner_ability.h"

class AbilityManager {
public:
    AbilityManager();
    std::unique_ptr<Ability> getAbility();
    void addAbility();
    void printAbilities() const;

private:
    std::vector<std::unique_ptr<Ability>> abilities_;
};

#endif // ABILITY_MANAGER_H