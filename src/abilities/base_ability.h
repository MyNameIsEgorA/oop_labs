#ifndef BASE_ABILITY_H
#define BASE_ABILITY_H

#include <iostream>
#include "../entities/field/field.h"

class Ability {
public:
    virtual ~Ability() = default;
    virtual void apply(Field& field) const = 0;
    virtual void printAbility() const = 0;
    [[nodiscard]] virtual bool needsCoordinates() const { return true; }
};

#endif