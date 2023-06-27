#pragma once

#include <string>

#include "token.hpp"

struct Error {
    Token token;
    std::string message;
    inline static bool had_error = false;
    Error(Token token, std::string message);
    static void report(int line, std::string where, std::string message);
};