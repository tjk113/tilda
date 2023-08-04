#pragma once

#include <iostream>
#include <optional>
#include <memory>
#include <vector>
#include <any>

#include "common.hpp"
#include "token.hpp"

struct Expression;
struct UnaryExpression;
struct BinaryExpression;
struct TernaryExpression;
struct LiteralExpression;
struct GroupExpression;
struct AssignExpression;
struct RangeExpression;
struct AccessExpression;
struct CallExpression;
struct LogicalExpression;
struct BitwiseExpression;

typedef std::shared_ptr<Expression> ShrExprPtr;
typedef std::shared_ptr<UnaryExpression> ShrUnaryExprPtr;
typedef std::shared_ptr<BinaryExpression> ShrBinaryExprPtr;
typedef std::shared_ptr<TernaryExpression> ShrTernaryExprPtr;
typedef std::shared_ptr<LiteralExpression> ShrLiteralExprPtr;
typedef std::shared_ptr<GroupExpression> ShrGroupExprPtr;
typedef std::shared_ptr<AssignExpression> ShrAssignExprPtr;
typedef std::shared_ptr<RangeExpression> ShrRangeExprPtr;
typedef std::shared_ptr<AccessExpression> ShrAccessExprPtr;
typedef std::shared_ptr<CallExpression> ShrCallExprPtr;
typedef std::shared_ptr<LogicalExpression> ShrLogicalExprPtr;
typedef std::shared_ptr<BitwiseExpression> ShrBitwiseExprPtr;

template<typename T>
struct ExpressionVisitor {
    virtual T visit_unary_expression(ShrUnaryExprPtr expression) = 0;
    virtual T visit_binary_expression(ShrBinaryExprPtr expression) = 0;
    virtual T visit_ternary_expression(ShrTernaryExprPtr expression) = 0;
    virtual T visit_literal_expression(ShrLiteralExprPtr expression) = 0;
    virtual T visit_group_expression(ShrGroupExprPtr expression) = 0;
    virtual T visit_assign_expression(ShrAssignExprPtr expression) = 0;
    virtual T visit_range_expression(ShrRangeExprPtr expression) = 0;
    virtual T visit_access_expression(ShrAccessExprPtr expression) = 0;
    virtual T visit_call_expression(ShrCallExprPtr expression) = 0;
    virtual T visit_logical_expression(ShrLogicalExprPtr expression) = 0;
    virtual T visit_bitwise_expression(ShrBitwiseExprPtr expression) = 0;
    virtual ~ExpressionVisitor() = default;
};

struct Expression {
    virtual std::any accept(ExpressionVisitor<std::any>& expression_visitor) = 0;
    virtual ~Expression() = default;
};

struct UnaryExpression : Expression, public std::enable_shared_from_this<UnaryExpression> {
    TokenType type;
    ShrExprPtr operand;
    bool postfix;

    UnaryExpression(TokenType type, ShrExprPtr operand, bool postfix);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct BinaryExpression : Expression, public std::enable_shared_from_this<BinaryExpression> {
    TokenType type;
    ShrExprPtr l_operand;
    ShrExprPtr r_operand;

    BinaryExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct TernaryExpression : Expression, public std::enable_shared_from_this<TernaryExpression> {
    TokenType type;
    ShrExprPtr condition;
    ShrExprPtr l_operand;
    ShrExprPtr r_operand;

    TernaryExpression(TokenType type, ShrExprPtr condition, ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct LiteralExpression : Expression, public std::enable_shared_from_this<LiteralExpression> {
    TokenType type;
    std::any value;

    LiteralExpression(TokenType type, std::any value);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct GroupExpression : Expression, public std::enable_shared_from_this<GroupExpression> {
    ShrExprPtr expression;

    GroupExpression(ShrExprPtr expression);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct DeclareExpression : Expression, public std::enable_shared_from_this<DeclareExpression> {
    LiteralType type_specifier;
    Token identifier;
    bool is_const;
    ShrExprPtr expression;

    DeclareExpression(LiteralType type_specifier, Token identifier, bool is_const, ShrExprPtr expression);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
}

struct AssignExpression : Expression, public std::enable_shared_from_this<AssignExpression> {
    Token identifier;
    ShrExprPtr expression;

    AssignExpression(Token identifier, ShrExprPtr expression);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct RangeExpression : Expression, public std::enable_shared_from_this<RangeExpression> {
    ShrExprPtr l_operand;
    ShrExprPtr r_operand;

    RangeExpression(ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct AccessExpression : Expression, public std::enable_shared_from_this<AccessExpression> {
    ShrExprPtr l_operand;
    ShrExprPtr r_operand;

    AccessExpression(ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct CallExpression : Expression, public std::enable_shared_from_this<CallExpression> {
    /* this would have to be an ShrExprPtr if
    function pointers were added to the language */
    Token function_name;
    std::vector<ShrExprPtr> arguments;

    CallExpression(Token function_name, std::vector<ShrExprPtr> arguments);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct LogicalExpression : Expression, public std::enable_shared_from_this<LogicalExpression> {
    TokenType type;
    ShrExprPtr l_operand;
    // r_operand is optional for LogicalExpressions
    ShrExprPtr r_operand;

    LogicalExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};

struct BitwiseExpression : Expression, public std::enable_shared_from_this<BitwiseExpression> {
    TokenType type;
    ShrExprPtr l_operand;
    // r_operand is optional for BitwiseExpressions
    ShrExprPtr r_operand;

    BitwiseExpression(TokenType type, ShrExprPtr l_operand, ShrExprPtr r_operand);
    std::any accept(ExpressionVisitor<std::any>& expression_visitor) override;
};