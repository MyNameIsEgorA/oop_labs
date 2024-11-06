#ifndef FIELD_EXCEPTIONS
#define FIELD_EXCEPTIONS

#include "../../base_exception.h"

class AttackOutOfRangeException : public BaseException {
public:
    AttackOutOfRangeException(const std::string& message = "Атака за пределы поля") : BaseException(message) {}
};

class ShipPlacementOutOfRange : public BaseException {
public:
    ShipPlacementOutOfRange(const std::string& message = "Нельзя ставить корабль вне поля") : BaseException(message) {}
};

#endif