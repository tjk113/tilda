#pragma once

#include <concepts>
#include <vector>
#include <string>

#include "expression.hpp"
#include "token.hpp"

struct Parser {
    std::vector<Token> tokens;
    int current = 0;

    Parser(std::vector<Token> tokens);

    void test();
    ShrExprPtr parse();
    Token previous();
    Token next();
    Token peek();
    Token peek_next();
    bool is_at_end();
    template<std::same_as<TokenType>... T>
    bool match(T... types);
    template<std::same_as<TokenType>... T>
    bool match_next(T... types);
    Token consume(TokenType type, std::string message);
    void throw_error(Token token, std::string message);
    void synchronize();
    // Expression handlers (it was here that i realized why i needed smart pointers...)
    ShrExprPtr handle_expression();
    ShrExprPtr handle_ternary();
    ShrExprPtr handle_equality();
    ShrExprPtr handle_bitwise();
    ShrExprPtr handle_comparison();
    ShrExprPtr handle_term();
    ShrExprPtr handle_factor();
    ShrExprPtr handle_unary();
    ShrExprPtr handle_unary_postfix();
    ShrExprPtr handle_primary();
};