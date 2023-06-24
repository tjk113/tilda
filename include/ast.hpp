#pragma once

#include <variant>
#include <string>
#include <memory>
#include <vector>

#include "expression.hpp"
#include "statement.hpp"
#include "common.hpp"
#include "token.hpp"

struct AST : ExpressionVisitor<std::any> {
    /* Nodes are an Expression or a Statement,
    and hold a vector of subsequent Node refs */
    struct Node {
        std::variant<const Expression*, const Statement*> type;
        std::vector<Node*> subnodes;

        Node() = default;
        Node(std::variant<const Expression*, const Statement*> type);
    };

    AST() = default;
    
    std::string print(const Expression* expression);
    template<class... E>
    std::string parenthesize(std::string label, E... expressions);
    static void test();

    std::any visit_unary_expression(const UnaryExpression* expression);
    std::any visit_binary_expression(const BinaryExpression* expression);
    std::any visit_literal_expression(const LiteralExpression* expression);
    std::any visit_group_expression(const GroupExpression* expression);
    std::any visit_assign_expression(const AssignExpression* expression);
    std::any visit_range_expression(const RangeExpression* expression);
    std::any visit_access_expression(const AccessExpression* expression);
    std::any visit_call_expression(const CallExpression* expression);
    std::any visit_logical_expression(const LogicalExpression* expression);
    std::any visit_bitwise_expression(const BitwiseExpression* expression);
};