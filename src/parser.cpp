#include <iostream>
#include <memory>
#include <vector>

#include "expression.hpp"
#include "common.hpp"
#include "parser.hpp"
#include "tilda.hpp"
#include "token.hpp"
#include "error.hpp"

Parser::Parser(std::vector<Token> tokens) :
    tokens(tokens) {}

void Parser::test() {
    if (tokens.empty()) {
        DEBUG("Parser::test: tokens is empty!");
        return;
    }
    if (match(NUM, ADD, SUB))
        DEBUG("congrats mark");
}

ShrExprPtr Parser::parse() {
    ShrExprPtr expression = handle_expression();
    if (Error::had_error)
        return nullptr;
    return expression;
}

Token Parser::previous() {
    return current > 0 ? tokens[current - 1] : tokens[0];
}

Token Parser::next() {
    if (!is_at_end())
        current++;
    return previous();
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::peek_next() {
    return tokens[current + 1];
}

bool Parser::is_at_end() {
    return tokens[current].type == END_TOKEN;
}

// TODO: make this function not disgusting
template<std::same_as<TokenType>... T>
bool Parser::match(T... types) {
    bool is_match = false;
    std::vector<TokenType> types_to_match;
    (types_to_match.emplace_back(types), ...);

    for (TokenType type : types_to_match) {
        if (peek().type == type) {
            is_match = true;
            break;
        }
    }
    if (is_match && !is_at_end())
        next();

    return is_match;
}

Token Parser::consume(TokenType type, std::string message) {
    if (peek().type == type)
        return next();
    throw_error(peek(), message);
    return Token(); // NULL Token
}

void Parser::throw_error(Token token, std::string message) {
    if (token.type == END_TOKEN)
        Error::report(token.line, "at end", message);
    else {
        Error::report(token.line, "at " + token.lexeme, message);
    }
}

void Parser::synchronize() {
    next();
    while (!is_at_end()) {
        /* Because this synchronizes between statements,
        it needs to find the next statement after the one
        that threw the error. So it switches on possible
        tokens that would start a new statement */
        switch(peek().type) {
            case STRUCT:
            case TYPE:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }
    }
    next();
}

ShrExprPtr Parser::handle_expression() {
    return handle_ternary();
}

ShrExprPtr Parser::handle_ternary() {
    ShrExprPtr expression = handle_equality();

    if (match(T_IF)) {
        TokenType type = previous().type == T_IF ? TERN : previous().type; // in case somehow another ternary operator is added to the lang
        ShrExprPtr l_expression = handle_equality();
        consume(T_ELSE, "Expected ':' after expression.");
        ShrExprPtr r_expression = handle_expression();
        expression = std::make_shared<TernaryExpression>(type, expression, l_expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_equality() {
    ShrExprPtr expression = handle_comparison();

    while (match(EQ, NOT_EQ)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_comparison();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_comparison() {
    ShrExprPtr expression = handle_term();

    while (match(GREATER, GREATER_EQ, LESS, LESS_EQ)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_term();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_term() {
    ShrExprPtr expression = handle_factor();

    while (match(SUB, ADD)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_factor();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_factor() {
    ShrExprPtr expression = handle_unary();

    while (match(DIV, MUL)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_unary();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_unary() {
    /* Try to handle a unary expression.
    If it doesn't find one, then handle
    the expression of next highest precedence
    (primary expression) */
    if (match(L_NOT, NEG)) {
        TokenType type = previous().type;
        ShrExprPtr expression = handle_unary();
        return std::make_shared<UnaryExpression>(type, expression);
    }
    return handle_primary();
}

ShrExprPtr Parser::handle_primary() {
    ShrExprPtr expression;
    if (match(IDENTIFIER, TYPE, NUM, STR, TRUE, FALSE, L_PAREN/*, NIL*/)) {
        TokenType type = previous().type;
        // Handle grouped expression
        if (type == L_PAREN) {
            ShrExprPtr grouped_expression = handle_expression();
            consume(R_PAREN, "Expected ')' after expression.");
            expression = grouped_expression;
        }
        else
            expression = std::make_shared<LiteralExpression>(type, previous().literal);
    }
    else {
        throw_error(peek(), "Expected expression.");
        return nullptr;
    }
    return expression;
}