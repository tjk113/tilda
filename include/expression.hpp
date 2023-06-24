#pragma once

#include <iostream>
#include <vector>
#include <any>

#include "token.hpp"

struct UnaryExpression;
struct BinaryExpression;
struct LiteralExpression;
struct GroupExpression;
struct AssignExpression;
struct RangeExpression;
struct AccessExpression;
struct CallExpression;
struct LogicalExpression;
struct BitwiseExpression;

template<typename T>
struct ExpressionVisitor {
    virtual T visit_unary_expression(const UnaryExpression* expression) = 0;
    virtual T visit_binary_expression(const BinaryExpression* expression) = 0;
    virtual T visit_literal_expression(const LiteralExpression* expression) = 0;
    virtual T visit_group_expression(const GroupExpression* expression) = 0;
    virtual T visit_assign_expression(const AssignExpression* expression) = 0;
    virtual T visit_range_expression(const RangeExpression* expression) = 0;
    virtual T visit_access_expression(const AccessExpression* expression) = 0;
    virtual T visit_call_expression(const CallExpression* expression) = 0;
    virtual T visit_logical_expression(const LogicalExpression* expression) = 0;
    virtual T visit_bitwise_expression(const BitwiseExpression* expression) = 0;
    virtual ~ExpressionVisitor() = default;
};

struct Expression {
    virtual std::any accept(ExpressionVisitor<std::any>& expression_visitor) const = 0;
    virtual ~Expression() = default;
};

struct UnaryExpression : Expression {
    TokenType type;
    const Expression* operand;

    UnaryExpression(TokenType type, const Expression* operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct BinaryExpression : Expression {
    TokenType type;
    const Expression* l_operand;
    const Expression* r_operand;

    BinaryExpression(TokenType type, const Expression* l_operand, const Expression* r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct LiteralExpression : Expression {
    TokenType type;
    std::any value;

    LiteralExpression(TokenType type, std::any value);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct GroupExpression : Expression {
    const Expression* expression;

    GroupExpression(const Expression* expression);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct AssignExpression : Expression {
    Token identifier;
    const Expression* expression;

    AssignExpression(Token identifier, const Expression* expression);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct RangeExpression : Expression {
    const Expression* l_operand;
    const Expression* r_operand;

    RangeExpression(const Expression* l_operand, const Expression* r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct AccessExpression : Expression {
    const Expression* l_operand;
    const Expression* r_operand;

    AccessExpression(const Expression* l_operand, const Expression* r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct CallExpression : Expression {
    /* this would have to be an Expression* if
    function pointers were added to the language */
    Token function_name;
    std::vector<const Expression*> arguments;

    CallExpression(Token function_name, std::vector<const Expression*> arguments);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

/* TODO: decide whether to create UnaryLogicalExpression,
BinaryLogicalExpression, UnaryBitwiseExpression, and
BinaryBitwiseExpression structs, or flatten them into
UnaryExpression and BinaryExpression */

struct LogicalExpression : Expression {
    TokenType type;
    const Expression* l_operand;
    const Expression* r_operand;

    LogicalExpression(TokenType type, const Expression* l_operand, const Expression* r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};

struct BitwiseExpression : Expression {
    TokenType type;
    const Expression* l_operand;
    const Expression* r_operand;

    BitwiseExpression(TokenType type, const Expression* l_operand, const Expression* r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) const override;
};