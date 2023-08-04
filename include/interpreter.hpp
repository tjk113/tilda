#pragma once

#include <vector>
#include <any>

#include "expression.hpp"

struct Interpreter : ExpressionVisitor<std::any> {

    Interpreter();
    std::any evaluate(ShrExprPtr expression);
    bool get_truthiness(std::any operand);
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