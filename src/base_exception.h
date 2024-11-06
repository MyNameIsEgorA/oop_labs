#ifndef EXCEPTION_BASE_H
#define EXCEPTION_BASE_H

#include <exception>
#include <string>

class BaseException : public std::exception {
public:
    BaseException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
protected:
    std::string message;
};

#endif // EXCEPTION_BASE_H