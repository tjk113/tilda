#pragma once

#include <string>

#include "token.hpp"

struct Error {
    // Token token;
    // std::string message;
    // Error(Token token, std::string message);
    static void report(int line, std::string where, std::string message);
};