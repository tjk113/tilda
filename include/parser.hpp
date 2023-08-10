#pragma once

#include <concepts>
#include <vector>
#include <string>

#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    void test();
    Token previous();
    Token next();
    Token peek();
    Token peek_next();
    bool check(TokenType type);
    bool is_at_end();
    template<std::same_as<TokenType>... T>
    bool match(T... types);
    template<std::same_as<TokenType>... T>
    bool match_next(T... types);
    Token consume(TokenType type, std::string message);
    void throw_error(Token token, std::string message);
    void synchronize();
    // Expression handlers (it was here that i realized why i needed smart pointers...)
    ShrStmtPtr handle_declaration();
    ShrStmtPtr handle_variable();
    ShrStmtPtr handle_statement();
    ShrStmtPtr handle_print();
    ShrStmtPtr handle_type();
    std::vector<ShrStmtPtr> handle_block();
    ShrStmtPtr handle_expression_statement();
    ShrExprPtr handle_expression();
    ShrExprPtr handle_assignment();
    ShrExprPtr handle_ternary();
    ShrExprPtr handle_equality();
    ShrExprPtr handle_bitwise();
    ShrExprPtr handle_comparison();
    ShrExprPtr handle_term();
    ShrExprPtr handle_factor();
    ShrExprPtr handle_unary();
    ShrExprPtr handle_unary_postfix();
    ShrExprPtr handle_primary();
public:
    Parser(std::vector<Token> tokens);
    std::vector<ShrStmtPtr> parse();
};