#include "expression.hpp"
#include <any>

UnaryExpression::UnaryExpression(TokenType type, const Expression* operand) :
    type(type), operand(operand) {}

std::any UnaryExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_unary_expression(this);
}

BinaryExpression::BinaryExpression(TokenType type, const Expression* l_operand, const Expression* r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any BinaryExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_binary_expression(this);
}

LiteralExpression::LiteralExpression(TokenType type, std::any value) :
    type(type), value(value) {}

std::any LiteralExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_literal_expression(this);
}

GroupExpression::GroupExpression(const Expression* expression) :
    expression(expression) {}

std::any GroupExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_group_expression(this);
}

AssignExpression::AssignExpression(Token identifier, const Expression* expression) :
    identifier(identifier), expression(expression) {}

std::any AssignExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_assign_expression(this);
}

RangeExpression::RangeExpression(const Expression* l_operand, const Expression* r_operand) :
    l_operand(l_operand), r_operand(r_operand) {}

std::any RangeExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_range_expression(this);
}

AccessExpression::AccessExpression(const Expression* l_operand, const Expression* r_operand) :
    l_operand(l_operand), r_operand(r_operand) {}

std::any AccessExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_access_expression(this);
}

CallExpression::CallExpression(Token function_name, std::vector<const Expression*> arguments) :
    function_name(function_name), arguments(arguments) {}

std::any CallExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_call_expression(this);
}

LogicalExpression::LogicalExpression(TokenType type, const Expression* l_operand, const Expression* r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any LogicalExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_logical_expression(this);
}

BitwiseExpression::BitwiseExpression(TokenType type, const Expression* l_operand, const Expression* r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any BitwiseExpression::accept(ExpressionVisitor<std::any>& visitor) const {
    return visitor.visit_bitwise_expression(this);
}
