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
    
    std::string print(ShrExprPtr expression);
    template<class... E>
    std::string parenthesize(std::string label, E... expressions);
    static void test();

    std::any visit_unary_expression(ShrUnaryExprPtr expression);
    std::any visit_binary_expression(ShrBinaryExprPtr expression);
    std::any visit_ternary_expression(ShrTernaryExprPtr expression);
    std::any visit_literal_expression(ShrLiteralExprPtr expression);
    std::any visit_group_expression(ShrGroupExprPtr expression);
    std::any visit_assign_expression(ShrAssignExprPtr expression);
    std::any visit_range_expression(ShrRangeExprPtr expression);
    std::any visit_access_expression(ShrAccessExprPtr expression);
    std::any visit_call_expression(ShrCallExprPtr expression);
    std::any visit_logical_expression(ShrLogicalExprPtr expression);
    std::any visit_bitwise_expression(ShrBitwiseExprPtr expression);
};