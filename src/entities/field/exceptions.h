#ifndef FIELD_EXCEPTIONS
#define FIELD_EXCEPTIONS

#include "../../base_exception.h"

class AttackOutOfRangeException final : public BaseException {
public:
    explicit AttackOutOfRangeException(const std::string& message = "Атака за пределы поля") : BaseException(message) {}
};

class ShipPlacementOutOfRangeException final : public BaseException {
public:
    explicit ShipPlacementOutOfRangeException(const std::string& message = "Нельзя ставить корабль вне поля") : BaseException(message) {}
};

class ShipIncorrectPlacementException final : public BaseException {
public:
    explicit ShipIncorrectPlacementException(const std::string& message = "Корабль поставлен слишком близко к другому") : BaseException(message) {}
};

#endif