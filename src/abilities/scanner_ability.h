#ifndef SCANNER_ABILITY_H
#define SCANNER_ABILITY_H

#include "./base_ability.h"

class Scanner : public Ability {
public:
    void apply(Field& field) const override;
    void printAbility() const override;
};

#endif // SCANNER_ABILITY_H