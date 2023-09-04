#pragma once

#include <memory>
#include <vector>
#include <string>
#include <any>

#include "environment.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"

struct Interpreter : ExpressionVisitor<std::any>, StatementVisitor {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(Environment());
    std::any evaluate(ShrExprPtr expression);
    void execute(ShrStmtPtr statement);
    void execute_block(std::vector<ShrStmtPtr> statements, std::shared_ptr<Environment> environment);
    std::string get_type(ShrExprPtr expression);
    bool get_truthiness(std::any operand);
    bool get_equality(std::any l_operand, std::any r_operand);
    void throw_error(std::string message);
    bool check_number_operand(TokenType type, std::any operand);
    bool check_number_operands(TokenType type, std::any l_operand, std::any r_operand);
    std::string to_string(std::any value);
    std::any visit_unary_expression(ShrUnaryExprPtr expression);
    std::any visit_binary_expression(ShrBinaryExprPtr expression);
    std::any visit_ternary_expression(ShrTernaryExprPtr expression);
    std::any visit_literal_expression(ShrLiteralExprPtr expression);
    std::any visit_group_expression(ShrGroupExprPtr expression);
    std::any visit_variable_expression(ShrVariableExprPtr expression);
    std::any visit_assign_expression(ShrAssignExprPtr expression);
    std::any visit_range_expression(ShrRangeExprPtr expression);
    std::any visit_access_expression(ShrAccessExprPtr expression);
    std::any visit_call_expression(ShrCallExprPtr expression);
    std::any visit_logical_expression(ShrLogicalExprPtr expression);
    std::any visit_bitwise_expression(ShrBitwiseExprPtr expression);
    void visit_expression_statement(ShrExpressionStmtPtr statement);
    void visit_print_statement(ShrPrintStmtPtr statement);
    void visit_type_statement(ShrTypeStmtPtr statement);
    void visit_block_statement(ShrBlockStmtPtr statement);
    void visit_declare_statement(ShrDeclareStmtPtr statement);
    void visit_assign_statement(ShrAssignStmtPtr statement);
    void visit_if_statement(ShrIfStmtPtr statement);
    void visit_while_statement(ShrWhileStmtPtr statement);
    void visit_for_statement(ShrForStmtPtr statement);
    void visit_forin_statement(ShrForInStmtPtr statement);
    void visit_switch_statement(ShrSwitchStmtPtr statement);
    void visit_return_statement(ShrReturnStmtPtr statement);
    void visit_struct_statement(ShrStructStmtPtr statement);
public:
    Interpreter() = default;
    void interpret(std::vector<ShrStmtPtr> statements);
};