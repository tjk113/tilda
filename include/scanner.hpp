#pragma once

#include <stdbool.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <any>

#include "common.hpp"
#include "token.hpp"

class Scanner {
    std::string src;
    int start = 0;
    int current = 0;
    int line = 1;

    std::map<std::string, TokenType> keywords = {
        {"if", IF},
        {"else", ELSE},
        {"while", WHILE},
        {"for", FOR},
        {"in", IN},
        {"switch", SWITCH},
        {"return", RETURN},
        {"true", TRUE},
        {"false", FALSE},
        {"let", LET},
        {"const", CONST},
        {"struct", STRUCT},
        // TODO: add classes
    };

    std::string types[14] = {
        "u8", "i8", "u16", "i16",
        "u32", "i32", "int", "u64",
        "i64", "f32", "f64", "str",
        "void", "bool"
    };

public:
    Scanner(std::string src);
    std::vector<Token> tokens;
private:
    char next();
    char peek();
    char peek_next();
    bool is_at_end();
    void handle_string();
    void handle_number();
    void handle_identifier_or_type();
    void handle_two_char_operator(TokenType type, char next_char);
    void add_token(TokenType type);
    void add_token(TokenType type, std::any literal);
    void scan_token();
    void scan_tokens();
    void throw_error(std::string message);
};