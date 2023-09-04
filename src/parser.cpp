#include <iostream>
#include <cstdlib>
#include <format>
#include <memory>
#include <vector>

#include "expression.hpp"
#include "statement.hpp"
#include "common.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "tilda.hpp"
#include "token.hpp"
#include "error.hpp"

// TODO: where does type assignment to identifiers happen?

Parser::Parser(std::vector<Token> tokens) :
    tokens(tokens) {}

std::vector<ShrStmtPtr> Parser::parse() {
    std::vector<ShrStmtPtr> statements;
    while (!is_at_end())
        statements.push_back(handle_declaration());
    return statements;
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
    if (!is_at_end())
        return tokens[current + 1];
    return tokens[tokens.size() - 1];
}

bool Parser::check(TokenType type) {
    if (is_at_end()) return false;
    return peek().type == type;
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

template<std::same_as<TokenType>... T>
bool Parser::match_next(T... types) {
    bool is_match = false;
    std::vector<TokenType> types_to_match;
    (types_to_match.emplace_back(types), ...);

    for (TokenType type : types_to_match) {
        if (peek_next().type == type) {
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
    Tilda::had_error = true;
    throw std::format("[line {}] Error {}: {}", token.line, (token.type == END_TOKEN ? "at end" : "at "), message);
    // if (token.type == END_TOKEN)
    //     Error::report(token.line, "at end", message);
    // else
        // Error::report(token.line, "at " + token.lexeme, message);
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

ShrStmtPtr Parser::handle_declaration() {
    if (match(CONST, LET, TYPE))
        return handle_variable();
    else if (Tilda::had_error) {
        synchronize();
        return nullptr;
    }
    return handle_statement();
}

ShrStmtPtr Parser::handle_variable() {
    ShrExprPtr expression;
    bool is_const = previous().type == CONST ? true : false;
    LiteralType literal_type;

    // TODO: type inference
    if (previous().type == TYPE || match(TYPE))
        literal_type = StringToLiteralType::map[previous().lexeme];
    else if (previous().type == LET || match(LET))
        ;

    Token identifier = consume(IDENTIFIER, "Expected identifier.");

    if (match(ASSIGN))
        expression = handle_expression();
    
    consume(NEWLINE, "Expected newline after variable declaration.");

    return std::make_shared<DeclareStatement>(identifier, literal_type, expression);
}

ShrStmtPtr Parser::handle_statement() {
    if (match(IF)) return handle_if();
    if (match(PRINT)) return handle_print();
    if (match(TYPEOF)) return handle_type();
    if (match(L_BRACE)) return std::make_shared<BlockStatement>(handle_block());
    return handle_expression_statement();
}

ShrStmtPtr Parser::handle_if() {
    consume(L_PAREN, "Expected parenthesis after \"if\".");
    ShrExprPtr expression = handle_expression();
    consume(R_PAREN, "Expected parenthesis after expresion.");

    ShrStmtPtr then_branch = handle_statement();

    // TODO: elif statements

    ShrStmtPtr else_branch = nullptr;
    if (match(ELSE))
        else_branch = handle_statement();

    return std::make_shared<IfStatement>(expression, then_branch, else_branch);
}

ShrStmtPtr Parser::handle_print() {
    ShrExprPtr expression = handle_expression();
    consume(NEWLINE, "Expected newline after value.");
    return std::make_shared<PrintStatement>(expression);
}

ShrStmtPtr Parser::handle_type() {
    ShrExprPtr expression = handle_expression();
    consume(NEWLINE, "Expected newline after value.");
    return std::make_shared<TypeStatement>(expression);
}

std::vector<ShrStmtPtr> Parser::handle_block() {
    std::vector<ShrStmtPtr> statements;

    match(NEWLINE);

    while (!check(R_BRACE) && !is_at_end())
        statements.push_back(handle_declaration());
    consume(R_BRACE, "Expected \"}\" after block.");

    match(NEWLINE);
    
    return statements;
}

ShrStmtPtr Parser::handle_expression_statement() {
    ShrExprPtr expression = handle_expression();
    consume(NEWLINE, "Expected newline after expression.");
    return std::make_shared<ExpressionStatement>(expression);
}

ShrExprPtr Parser::handle_expression() {
    return handle_assignment();
}

ShrExprPtr Parser::handle_assignment() {
    ShrExprPtr expression = handle_logical_or();

    if (match(ASSIGN)) {
        Token target = previous();
        ShrExprPtr value = handle_assignment();

        if (VariableExpression* v = dynamic_cast<VariableExpression*>(expression.get())) {
            Token identifier = v->identifier;
            return std::make_shared<AssignExpression>(identifier, value);
        }
        // Report error
        std::cout << std::format("Invalid assignment target: \"{}\"", target.lexeme);
    }

    return expression;
}

ShrExprPtr Parser::handle_logical_or() {
    ShrExprPtr expression = handle_logical_and();

    while (match(L_OR)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_logical_and();
        expression = std::make_shared<LogicalExpression>(type, expression, r_expression);
    }

    return expression;
}

ShrExprPtr Parser::handle_logical_and() {
    ShrExprPtr expression = handle_logical_xor();

    while (match(L_AND)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_logical_xor();
        expression = std::make_shared<LogicalExpression>(type, expression, r_expression);
    }

    return expression;
}

ShrExprPtr Parser::handle_logical_xor() {
    ShrExprPtr expression = handle_ternary();

    while (match(L_XOR)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_ternary();
        expression = std::make_shared<LogicalExpression>(type, expression, r_expression);
    }

    return expression;
}

ShrExprPtr Parser::handle_ternary() {
    ShrExprPtr expression = handle_equality();

    if (match(T_IF)) {
        TokenType type = previous().type == T_IF ? TERN : previous().type; // in case somehow another ternary operator is added to the lang
        ShrExprPtr l_expression = handle_equality();
        consume(T_ELSE, "Expected ':' after expression.");
        ShrExprPtr r_expression = handle_ternary();
        expression = std::make_shared<TernaryExpression>(type, expression, l_expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_equality() {
    ShrExprPtr expression = handle_comparison();

    if (match(EQ, NOT_EQ)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_comparison();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_comparison() {
    ShrExprPtr expression = handle_bitwise();

    if (match(GREATER, GREATER_EQ, LESS, LESS_EQ)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_bitwise();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_bitwise() {
    ShrExprPtr expression = handle_term();

    if (match(TokenType::B_OR, B_AND, B_XOR, CHK, LSHFT, RSHFT)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_bitwise();
        expression = std::make_shared<BitwiseExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_term() {
    ShrExprPtr expression = handle_factor();

    if (match(SUB, ADD)) {
        TokenType type = previous().type;
        ShrExprPtr r_expression = handle_factor();
        expression = std::make_shared<BinaryExpression>(type, expression, r_expression);
    }
    return expression;
}

ShrExprPtr Parser::handle_factor() {
    ShrExprPtr expression = handle_unary();

    if (match(DIV, MUL, POW, MOD)) {
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
    if (match(L_NOT, B_NOT, NEG, INC, DEC)) {
        TokenType type = previous().type;
        ShrExprPtr expression = handle_unary();
        return std::make_shared<UnaryExpression>(type, expression, false);
    }
    // Don't allow chaining INC and DEC operators
    if (match(INC, DEC)) {
        TokenType type = previous().type;
        ShrExprPtr expression = handle_primary();
        return std::make_shared<UnaryExpression>(type, expression, false);
    }
    return handle_unary_postfix();
}

// idea to handle postfix operators like this stolen from https://github.com/svtm/jlox/
ShrExprPtr Parser::handle_unary_postfix() {
    if (match_next(INC, DEC)) {
        TokenType type = peek().type;
        current--;
        ShrExprPtr expression = handle_primary();
        next();
        return std::make_shared<UnaryExpression>(type, expression, true);
    }
    return handle_primary();
}

ShrExprPtr Parser::handle_primary() {
    ShrExprPtr expression;
    if (match(TYPE, NUM, STR, TRUE, FALSE, L_PAREN/*, NIL*/)) {
        TokenType type = previous().type;
        // Handle grouped expression
        if (type == L_PAREN) {
            ShrExprPtr grouped_expression = handle_expression();
            consume(R_PAREN, "Expected ')' after expression.");
            expression = grouped_expression;
        }
        else {
            expression = std::make_shared<LiteralExpression>(type, previous().literal);
        }
    }
    else if (match(IDENTIFIER))
        return std::make_shared<VariableExpression>(previous());
    else {
        throw_error(peek(), "Expected expression.");
        return nullptr;
    }
    return expression;
}