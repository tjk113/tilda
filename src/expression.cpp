#include "expression.hpp"

UnaryExpression::UnaryExpression(TokenType type, const Expression& operand) :
    type(type), operand(operand) {}

BinaryExpression::BinaryExpression(TokenType type, const Expression& l_operand,
                                   const Expression& r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

LiteralExpression::LiteralExpression(TokenType type, std::any value) :
    type(type), value(value) {}

GroupExpression::GroupExpression(const Expression& expression) :
    expression(expression) {}