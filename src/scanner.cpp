#include <iostream>
#include <format>
#include <string>
#include <any>

#include "scanner.hpp"
#include "token.hpp"

Scanner::Scanner(std::string src) {
    this->src = src;
    scan_tokens();
}

/* Advance the loop by getting the next
character in the source code */
char Scanner::next() {
    current++;
    return src.at(current - 1);
}

/* Observes the current character without
advancing the character pointer */
char Scanner::peek() {
    return src.at(current - 1);
}

/* Observes the next character without
advancing the character pointer */
char Scanner::peek_next() {
    if (current + 1 >= src.length())
        return '\0';
    return src.at(current + 1);
}

/* Returns true if the current character
is the last one in the source code */
bool Scanner::is_at_end() {
    return current >= src.length();
}

void Scanner::handle_string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek_next() == '\n')
            line++;
        next();
    }
    if (is_at_end()) {
        throw_error("Unterminated string!");
        return;
    }
    // Consume ending quote
    next();

    std::string lexeme = src.substr(start + 1, current - 2);
    add_token(STR, lexeme);
}

void Scanner::handle_number() {
    while (isdigit(peek()) && !is_at_end()) {
        std::cout << peek() << std::endl;
        next();
    }

    // Look for a fractional part
    if (peek() == '.' && isdigit(peek_next())) {
        // Just consume that '.'
        next();
        while (isdigit(peek()) && !is_at_end())
            next();
    }

    add_token(NUM);
}

// TODO: unicode support
void Scanner::handle_identifier_or_type() {
    while (isalnum(peek()) && !is_at_end()) {
        next();
    }

    std::string lexeme = src.substr(start, current);
    TokenType type;

    auto found_keyword = keywords.find(lexeme);
    if (found_keyword != keywords.end())
        type = found_keyword->second;
    else {
        for (std::string data_type : types) {
            if (lexeme == data_type)
                type = TYPE;
        }
        if (type != TYPE)
            type = IDENTIFIER;
    }

    add_token(type);
}

void Scanner::add_token(TokenType type) {
    add_token(type, NULL);
}

void Scanner::add_token(TokenType type, std::any literal) {
    std::string lexeme = src.substr(start, current - start);
    tokens.emplace_back(Token(type, lexeme, literal, line));
}

void Scanner::scan_token() {
    char c = next();
    switch (c) {
        case '(': add_token(L_PAREN); break;
        case ')': add_token(R_PAREN); break;
        case '{': add_token(L_BRACE); break;
        case '}': add_token(R_BRACE); break;
        case '[': add_token(L_BRACKET); break;
        case ']': add_token(R_BRACKET); break;
        case '"': handle_string(); break;
        case ',': add_token(COMMA); break;
        case '+': peek() == '+' ? add_token(INC) : add_token(ADD); break;
        case '-': peek() == '-' ? add_token(DEC) : isdigit(peek()) ? add_token(NEG) : add_token(SUB); break;
        case '*': peek() == '*' ? add_token(POW) : add_token(MUL); break;
        case '/':
            if (peek() == '/') {
                while (peek() != '\n' && !is_at_end())
                    next();
            }
            else
                add_token(DIV);
            break;
        case '%': add_token(MOD); break;
        /* TODO: should "\" really be the
        integer division operator? */
        case '\\': add_token(INT_DIV); break;
        case '.': peek() == '.' ? add_token(RANGE) : add_token(ACCESS); break;
        case '=': peek() == '=' ? add_token(EQ) : add_token(ASSIGN); break;
        case '<': peek() == '=' ? add_token(LESS_EQ) : peek() == '<' ? add_token(LSHFT) : add_token(LESS); break;
        case '>': peek() == '=' ? add_token(GREATER_EQ) : peek() == '>' ? add_token(RSHFT) : add_token(GREATER); break;
        case '!': peek() == '=' ? add_token(NOT_EQ) : add_token(L_NOT); break;
        case '|': peek() == '|' ? add_token(L_OR) : add_token(B_OR); break;
        case '&': peek() == '&' ? add_token(L_AND) : add_token(B_AND); break;
        case '^': peek() == '^' ? add_token(L_XOR) : add_token(B_XOR); break;
        case '~': add_token(B_NOT); break;
        case '@': add_token(CHK); break;
        // Ignore whitespace
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': line++; break;
        default:
            if (isdigit(c))
                handle_number();
            // TODO: unicode support
            else if (isalnum(c))
                handle_identifier_or_type();
            else
                throw_error("Unexpected character!");
    }
}

void Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }
}

void Scanner::throw_error(std::string message) {
    std::cout << std::format("Error on line {}: {}", line, message) << std::endl;
}