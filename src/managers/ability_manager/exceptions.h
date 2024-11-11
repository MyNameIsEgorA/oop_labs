#ifndef ABILITIES_EXCEPTIONS_H
#define ABILITIES_EXCEPTIONS_H

#include "../../base_exception.h"

class EmptyAbilityException final : public BaseException {
public:
    explicit EmptyAbilityException(const std::string& message = "К сожалению, у вас не осталось способсностей") : BaseException(message) {}
};

#endif //ABILITIES_EXCEPTIONS_H
