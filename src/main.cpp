#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>

#include "interpreter.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "tilda.hpp"
#include "token.hpp"

void print_token_values(std::vector<Token> toks) {
    for (Token tok : toks)
        std::cout << Token::token_type_names[tok.type] << ": " << tok.lexeme << std::endl;
}

std::string get_statement_type(ShrStmtPtr statement) {
    if (typeid(*statement) == typeid(Statement))
        return "Statement";
    else if (typeid(*statement) == typeid(ExpressionStatement))
        return "ExpressionStatement";
    else if (typeid(*statement) == typeid(PrintStatement))
        return "PrintStatement";
    else if (typeid(*statement) == typeid(TypeStatement))
        return "TypeStatement";
    else if (typeid(*statement) == typeid(BlockStatement))
        return "BlockStatement";
    else if (typeid(*statement) == typeid(DeclareStatement))
        return "DeclareStatement";
    else if (typeid(*statement) == typeid(IfStatement))
        return "IfStatement";
    else if (typeid(*statement) == typeid(WhileStatement))
        return "WhileStatement";
    else if (typeid(*statement) == typeid(ForStatement))
        return "ForStatement";
    else if (typeid(*statement) == typeid(ForInStatement))
        return "ForInStatement";
    else if (typeid(*statement) == typeid(SwitchStatement))
        return "SwitchStatement";
    else if (typeid(*statement) == typeid(ReturnStatement))
        return "ReturnStatement";
    else if (typeid(*statement) == typeid(StructStatement))
        return "StructStatement";
    else
        return "Unknown Statement type";
}

void from_file(std::string path) {
    std::fstream fstream(path);
    std::string src;

    Interpreter interpreter;

    if (fstream) {
        std::ostringstream stringstream;
        stringstream << fstream.rdbuf() << "\n";
        src = stringstream.str();
    }
    else {
        std::cout << "Input file could not be read" << std::endl;
        return;
    }

    try {
        Scanner scanner(src);
        Parser parser(scanner.tokens);
        std::vector<ShrStmtPtr> statements = parser.parse();
        interpreter.interpret(statements);
    }
    catch (std::string message) {
        std::cout << message << std::endl;
    }
    Tilda::had_error = Tilda::had_runtime_error = false;
}

void from_repl() {
    std::string line;
    std::cout << "tilda ~ alpha v0.1" << std::endl;

    Interpreter interpreter;

    // Main loop
    while (std::cout << ">> " && std::getline(std::cin, line)) {
        if (line == "q")
            std::exit(0);
        // stdin consumes newline, so add it back
        line += "\n";
        // Handle empty line
        if (line == "\n")
            continue;

        try {
            Scanner scanner(line);
            Parser parser(scanner.tokens);
            std::vector<ShrStmtPtr> statements = parser.parse();
            // Typechecker typechecker(statements);
            interpreter.interpret(statements);
        }
        catch (std::string message) {
            std::cout << message << std::endl;
        }
        Tilda::had_error = Tilda::had_runtime_error = false;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2)
        std::cout << "usage: tilda [file]" << std::endl;
    else if (argc == 1) {
        from_repl();
    }
    else {
        from_file(argv[1]);
    }
}