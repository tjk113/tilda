#pragma once

#include <iostream>
#include <any>

#include "token.hpp"

// Gets source code name of literal
#define SRC_NAME(literal) #literal

struct Expression {
    virtual ~Expression() = default;
};

struct UnaryExpression : Expression {
    TokenType type;
    const Expression& operand;

    UnaryExpression() = default;
    UnaryExpression(TokenType type, const Expression& operand);
};

struct BinaryExpression : Expression {
    TokenType type;
    const Expression& l_operand;
    const Expression& r_operand;

    BinaryExpression() = default;
    BinaryExpression(TokenType type, const Expression& l_operand, const Expression& r_operand);
};

struct LiteralExpression : Expression {
    TokenType type;
    std::any value;

    LiteralExpression() = default;
    LiteralExpression(TokenType type, std::any value);
};

struct GroupExpression : Expression {
    const Expression& expression;

    GroupExpression() = default;
    GroupExpression(const Expression& expression);
};