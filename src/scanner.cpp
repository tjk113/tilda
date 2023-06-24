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
    return src.at(current);
}

/* Returns true if the current character
is the last one in the source code */
bool Scanner::is_at_end() {
    return current >= src.length();
}

void Scanner::handle_string() {
    do {
        if (peek_next() == '\n')
            line++;
        next();
    } while (peek() != '"' && !is_at_end());

    if (peek() != '"' && is_at_end()) {
        throw_error("Unterminated string!");
        return;
    }

    // Literal should cut off quotes
    std::string value = src.substr(start + 1, current - start - 2);
    add_token(STR, value);
}

void Scanner::handle_number() {
    while (isdigit(peek()) && !is_at_end())
        next();

    // Look for floating point
    if (peek() == '.' && isdigit(peek_next())) {
        // Just consume that '.'
        next();
        while (isdigit(peek()) && !is_at_end())
            next();
    }
    if (!is_at_end())
        current--;

    std::string t = src.substr(start, current - start);
    // TODO: should all numbers be initially considered doubles?
    add_token(NUM, std::stod(src.substr(start, current - start)));
}

// TODO: unicode support
void Scanner::handle_identifier_or_type() {
    while ((isalnum(peek()) || peek() == '_') && !is_at_end())
        next();
    /* So that we aren't capturing the extra
    character that the final next() call picks up */
    if (!is_at_end())
        current--;

    std::string lexeme = src.substr(start, current - start);
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

void Scanner::handle_two_char_operator(TokenType type, char next_char) {
    while (peek() == next_char && !is_at_end()) {
        next();
    }
    current--;
    add_token(type);
}

void Scanner::add_token(TokenType type) {
    add_token(type, NULL);
}

void Scanner::add_token(TokenType type, std::any literal) {
    std::string lexeme = src.substr(start, current - start);
    std::any value = literal.has_value() ? lexeme : literal;
    tokens.emplace_back(Token(type, lexeme, value, line));
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
        case '+': peek_next() == '+' ? handle_two_char_operator(INC, '+') : add_token(ADD); break;
        case '-': peek_next() == '-' ? handle_two_char_operator(DEC, '-') : isdigit(peek()) ? add_token(NEG) : add_token(SUB); break;
        case '*': peek_next() == '*' ? handle_two_char_operator(POW, '*') : add_token(MUL); break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !is_at_end())
                    next();
                if (peek() == '\n')
                    line++;
            }
            else
                add_token(DIV);
            break;
        case '%': add_token(MOD); break;
        /* TODO: should "\" really be the
        integer division operator? */
        case '\\': add_token(INT_DIV); break;
        case '.': peek_next() == '.' ? handle_two_char_operator(RANGE, '.') : add_token(ACCESS); break;
        case '=': peek_next() == '=' ? handle_two_char_operator(EQ, '=') : add_token(ASSIGN); break;
        case '<': peek_next() == '=' ? handle_two_char_operator(LESS_EQ, '=') : peek_next() == '<' ? handle_two_char_operator(LSHFT, '<') : add_token(LESS); break;
        case '>': peek_next() == '=' ? handle_two_char_operator(GREATER_EQ, '=') : peek_next() == '>' ? handle_two_char_operator(RSHFT, '>') : add_token(GREATER); break;
        case '!': peek_next() == '=' ? handle_two_char_operator(NOT_EQ, '=') : add_token(L_NOT); break;
        case '|': peek_next() == '|' ? handle_two_char_operator(L_OR, '|') : add_token(B_OR); break;
        case '&': peek_next() == '&' ? handle_two_char_operator(L_AND, '&') : add_token(B_AND); break;
        case '^': peek_next() == '^' ? handle_two_char_operator(L_XOR, '^') : add_token(B_XOR); break;
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