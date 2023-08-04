#include "expression.hpp"
#include <any>

UnaryExpression::UnaryExpression(TokenType type, ShrExprPtr operand, bool postfix) :
    type(type), operand(operand), postfix(postfix) {}

std::any UnaryExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_unary_expression(shared_from_this());
}

BinaryExpression::BinaryExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any BinaryExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_binary_expression(shared_from_this());
}

TernaryExpression::TernaryExpression(TokenType type, ShrExprPtr condition, ShrExprPtr l_operand, ShrExprPtr r_operand) :
    type(type), condition(condition), l_operand(l_operand), r_operand(r_operand) {}

std::any TernaryExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_ternary_expression(shared_from_this());
}

LiteralExpression::LiteralExpression(TokenType type, std::any value) :
    type(type), value(value) {}

std::any LiteralExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_literal_expression(shared_from_this());
}

GroupExpression::GroupExpression(ShrExprPtr expression) :
    expression(expression) {}

std::any GroupExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_group_expression(shared_from_this());
}

DeclareExpression::DeclareExpression(LiteralType type_specifier, Token identifier, bool is_const, ShrExprPtr expression) :
    type_specifier(type_specifier), identifier(identifier), is_const(is_const), expression(expression) {}

std::any DeclareExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_declare_expression(shared_from_this());
}

AssignExpression::AssignExpression(Token identifier, ShrExprPtr expression) :
    identifier(identifier), expression(expression) {}

std::any AssignExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_assign_expression(shared_from_this());
}

RangeExpression::RangeExpression(ShrExprPtr l_operand, ShrExprPtr r_operand) :
    l_operand(l_operand), r_operand(r_operand) {}

std::any RangeExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_range_expression(shared_from_this());
}

AccessExpression::AccessExpression(ShrExprPtr l_operand, ShrExprPtr r_operand) :
    l_operand(l_operand), r_operand(r_operand) {}

std::any AccessExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_access_expression(shared_from_this());
}

CallExpression::CallExpression(Token function_name, std::vector<ShrExprPtr> arguments) :
    function_name(function_name), arguments(arguments) {}

std::any CallExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_call_expression(shared_from_this());
}

LogicalExpression::LogicalExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any LogicalExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_logical_expression(shared_from_this());
}

BitwiseExpression::BitwiseExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand) :
    type(type), l_operand(l_operand), r_operand(r_operand) {}

std::any BitwiseExpression::accept(ExpressionVisitor<std::any>& visitor) {
    return visitor.visit_bitwise_expression(shared_from_this());
}
