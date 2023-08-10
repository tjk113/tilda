#include <iostream>
#include <map>

#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line) :
    type(type), lexeme(lexeme), literal(literal), line(line) {}

std::map<TokenType, std::string> Token::token_type_names = {
    // Syntax
    {L_PAREN, "("},
    {R_PAREN, ")"},
    {L_BRACE, "{"},
    {R_BRACE, "}"},
    {L_BRACKET, "["},
    {R_BRACKET, "]"},
    {COMMA, ","},
    {ASSIGN, "="},
    {RANGE, ".."},
    {ACCESS, "."},
    {COMMENT, "//"},
    // Keywords
    {IF, "if"},
    {ELSE, "else"},
    {WHILE, "while"},
    {FOR, "for"},
    {IN, "in"},
    {SWITCH, "switch"},
    {RETURN, "return"},
    {TRUE, "true"},
    {FALSE, "false"},
    {LET, "let"},
    {CONST, "const"},
    {STRUCT, "struct"},
    // Values
    {IDENTIFIER, "identifier"},
    {TYPE, "type"},
    {STR, "string"},
    {NUM, "number"},
    // Math Operators
    {ADD, "+"},
    {SUB, "-"},
    {MUL, "*"},
    {DIV, "/"},
    {INT_DIV, "\\"},
    {POW, "**"},
    {NEG, "-"},
    {MOD, "%"},
    {INC, "++"},
    {DEC, "--"},
    // Logical Operators
    {LESS, "<"},
    {GREATER, ">"},
    {LESS_EQ, "<="},
    {GREATER_EQ, ">="},
    {NOT_EQ, "!="},
    {EQ, "=="},
    {L_OR, "||"},
    {L_AND, "&&"},
    {L_XOR, "^^"},
    {L_NOT, "!"},
    // Ternary Logical Operators
    {TERN, "?:"},
    {T_IF, "?"},
    {T_ELSE, ":"},
    // Bitwise Operators
    {B_OR, "|"},
    {B_AND, "&"},
    {B_XOR, "^"},
    {B_NOT, "~"},
    {LSHFT, "<<"},
    {RSHFT, ">>"},
    {CHK, "@"},
    // Built-in Functions
    {PRINT, "print"},
    {TYPEOF, "type"},
    // End-of-file
    {END_TOKEN, "eof"}
};