#pragma once

#include <string>
#include <map>
#include <any>

#include "types.hpp"

enum TokenType {
    // Syntax
    L_PAREN, R_PAREN, L_BRACE, R_BRACE,
    L_BRACKET, R_BRACKET, COMMA, ASSIGN,
    RANGE, ACCESS, DEREF_ACESS, DEREF, COMMENT,
    // Keywords
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, 
    IN, SWITCH, RETURN, TRUE, FALSE,
    LET, CONST, STRUCT,
    // Values
    IDENTIFIER, TYPE, STR, NUM,
    // Math Operators
    ADD, SUB, MUL, DIV, INT_DIV, POW, NEG,
    MOD, INC, DEC,
    // Logical Operators
    LESS, GREATER, LESS_EQ, GREATER_EQ,
    NOT_EQ, EQ, L_OR, L_AND, L_XOR, L_NOT,
    // Ternary Logical Operators
    T_IF, T_ELSE, TERN, // not actually a tokenized operator
    // Bitwise Operators
    B_OR, B_AND, B_XOR, B_NOT,
    LSHFT, RSHFT, CHK,
    // Built-in Functions
    PRINT, PRINTLN,
    // End-of-file
    END_TOKEN
};

struct Token {
    TokenType type;
    LiteralType literal_type;
    // bool is_pointer;
    // bool is_reference;
    std::any literal;
    std::string lexeme;
    int line;

    Token() = default;
    Token(TokenType type, std::string lexeme, std::any literal, int line);

    static std::map<TokenType, std::string> token_type_names;
};