#include <stdint.h>
#include <iostream>
#include <variant>
#include <format>
#include <string>
#include <any>

#include "scanner.hpp"
#include "tilda.hpp"
#include "token.hpp"

Scanner::Scanner(std::string src) {
    this->src = src;
    scan_tokens();
}

/* Advance the loop by getting the next
character in the source code */
char Scanner::next() {
    if (!is_at_end())
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
    if (is_at_end())
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
    std::string literal = src.substr(start + 1, current - start - 2);
    add_token(STR, literal);
}

void Scanner::handle_number() {
    bool is_hex = false, is_binary = false, is_float = false;
    do {
        next();
    } while (isdigit(peek()) && !is_at_end());
        // next();

    // Look for floating point
    if (peek() == '.' && isdigit(peek_next())) {
        is_float = true;
        // Just consume that '.'
        next();
        while (isdigit(peek_next()) && !is_at_end())
            next();
    }
    // Look for 'x' denoter
    else if (peek() == 'x' && isalnum(peek_next())) {
        is_hex = true;
        // Consume!
        next();
        while (isalnum(peek_next()) && !is_at_end())
            next();
    }
    // Look for 'b' denoter
    else if (peek() == 'b' && isdigit(peek_next())) {
        is_binary = true;
        // Consume!
        next();
        while (isdigit(peek_next()) && !is_at_end())
            next();
    }

    if (!is_float && !is_hex && !is_binary)
        current--;

    std::variant<int64_t, double> literal;
    if (is_float)
        literal = std::stod(src.substr(start, current - start));
    else if (is_hex)
        literal = std::stoll(src.substr(start + 2, current - start - 2), nullptr, 16);
    else if (is_binary)
        literal = std::stoll(src.substr(start + 2, current - start - 2), nullptr, 2);
    else
        literal = std::stoll(src.substr(start, current - start));
    add_token(NUM, is_float ? std::any(std::get<double>(literal)) : std::any(std::get<int64_t>(literal)));
}

// TODO: unicode support
void Scanner::handle_identifier_or_type() {
    while ((isalnum(peek_next()) || peek_next() == '_') && !is_at_end())
        next();

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
        if (type != TYPE) {
            // TODO: remove
            if (lexeme == "print")
                type = PRINT;
            else if (lexeme == "type")
                type = TYPEOF;
            else
                type = IDENTIFIER;
        }
    }
    // bool handling (is this sloppy?)
    if (type == TRUE)
        add_token(type, true);
    else if (type == FALSE)
        add_token(type, false);
    else
        add_token(type);
}

void Scanner::handle_two_char_operator(TokenType type, char next_char) {
    while (peek_next() == next_char && !is_at_end())
        next();
    add_token(type);
}

void Scanner::add_token(TokenType type) {
    add_token(type, std::any()); // NULL std::any value
}

void Scanner::add_token(TokenType type, std::any literal) {
    std::string lexeme = src.substr(start, current - start);
    std::any value = literal.has_value() ? literal : lexeme;
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
        // TODO: how to handle whitespace between a negation operator and its operand?
        case '-': peek_next() == '-' ? handle_two_char_operator(DEC, '-') :
            (isalnum(peek_next()) || peek_next() == '(') ? add_token(NEG) : add_token(SUB); break;
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
        case ';': add_token(SEMICOLON); break;
        case '%': add_token(MOD); break;
        case '.': peek_next() == '.' ? handle_two_char_operator(RANGE, '.') : add_token(ACCESS); break;
        case '=': peek_next() == '=' ? handle_two_char_operator(EQ, '=') : add_token(ASSIGN); break;
        case '<': peek_next() == '=' ? handle_two_char_operator(LESS_EQ, '=') : peek_next() == '<' ? handle_two_char_operator(LSHFT, '<') : add_token(LESS); break;
        case '>': peek_next() == '=' ? handle_two_char_operator(GREATER_EQ, '=') : peek_next() == '>' ? handle_two_char_operator(RSHFT, '>') : add_token(GREATER); break;
        case '!': peek_next() == '=' ? handle_two_char_operator(NOT_EQ, '=') : add_token(L_NOT); break;
        case '|': peek_next() == '|' ? handle_two_char_operator(L_OR, '|') : add_token(B_OR); break;
        case '&': peek_next() == '&' ? handle_two_char_operator(L_AND, '&') : add_token(B_AND); break;
        case '^': peek_next() == '^' ? handle_two_char_operator(L_XOR, '^') : add_token(B_XOR); break;
        case '~': add_token(B_NOT); break;
        case '?': add_token(T_IF); break;
        case ':': add_token(T_ELSE); break;
        case '@': add_token(CHK); break;
        // Ignore whitespace
        case ' ':
        case '\r':
        // TODO: make this actually a line continue character
        case '\\':
        case '\t': break;
        case '\n': add_token(NEWLINE); line++; break;
        default:
            if (isdigit(c))
                handle_number();
            // TODO: unicode support
            else if (isalnum(c))
                handle_identifier_or_type();
            else
                throw_error(std::format("Unexpected character \"{}\"!", c));
    }
}

void Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }
    tokens.push_back(Token(END_TOKEN, "", NULL, line));
}

void Scanner::throw_error(std::string message) {
    Tilda::had_error = true;
    throw std::format("Error on line {}: {}", line, message);
}