#pragma once

#include <string>
#include <any>

#include "tokentype.hpp"

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token() = default;
    Token(TokenType type, std::string lexeme, std::any literal, int line);
};