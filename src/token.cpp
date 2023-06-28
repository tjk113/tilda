#include <iostream>
#include <map>

#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line) :
    type(type), lexeme(lexeme), literal(literal), line(line) {}

std::map<TokenType, std::string> Token::token_type_names = {
    // Syntax
    {L_PAREN, "L_PAREN"},
    {R_PAREN, "R_PAREN"},
    {L_BRACE, "L_BRACE"},
    {R_BRACE, "R_BRACE"},
    {L_BRACKET, "L_BRACKET"},
    {R_BRACKET, "R_BRACKET"},
    {COMMA, "COMMA"},
    {ASSIGN, "ASSIGN"},
    {RANGE, "RANGE"},
    {ACCESS, "ACCESS"},
    {COMMENT, "COMMENT"},
    // Keywords
    {IF, "IF"},
    {ELSE, "ELSE"},
    {WHILE, "WHILE"},
    {FOR, "FOR"},
    {IN, "IN"},
    {GOTO, "GOTO"},
    {SWITCH, "SWITCH"},
    {RETURN, "RETURN"},
    {TRUE, "TRUE"},
    {FALSE, "FALSE"},
    {LET, "LET"},
    {CONST, "CONST"},
    {STRUCT, "STRUCT"},
    // Values
    {IDENTIFIER, "IDENTIFIER"},
    {TYPE, "TYPE"},
    {STR, "STR"},
    {NUM, "NUM"},
    // Math Operators
    {ADD, "ADD"},
    {SUB, "SUB"},
    {MUL, "MUL"},
    {DIV, "DIV"},
    {INT_DIV, "INT_DIV"},
    {POW, "POW"},
    {NEG, "NEG"},
    {MOD, "MOD"},
    {INC, "INC"},
    {DEC, "DEC"},
    // Logical Operators
    {LESS, "LESS"},
    {GREATER, "GREATER"},
    {LESS_EQ, "LESS_EQ"},
    {GREATER_EQ, "GREATER_EQ"},
    {NOT_EQ, "NOT_EQ"},
    {EQ, "EQ"},
    {L_OR, "L_OR"},
    {L_AND, "L_AND"},
    {L_XOR, "L_XOR"},
    {L_NOT, "L_NOT"},
    // Ternary Logical Operators
    {TERN, "TERN"},
    {T_IF, "T_IF"},
    {T_ELSE, "T_ELSE"},
    // Bitwise Operators
    {B_OR, "B_OR"},
    {B_AND, "B_AND"},
    {B_XOR, "B_XOR"},
    {B_NOT, "B_NOT"},
    {LSHFT, "LSHFT"},
    {RSHFT, "RSHFT"},
    {CHK, "CHK"},
    // Built-in Functions
    {PRINT, "PRINT"},
    {PRINTLN, "PRINTLN"},
    // End-of-file
    {END_TOKEN, "EOF"}
};