#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>

#include "expression.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "tilda.hpp"
#include "token.hpp"
#include "ast.hpp"

void print_token_values(std::vector<Token> toks) {
    for (Token tok : toks)
        std::cout << Token::token_type_names[tok.type] << ": " << tok.lexeme << std::endl;
}

void from_file(std::string path) {
    std::fstream fstream(path);
    std::string src;

    if (fstream) {
        std::ostringstream stringstream;
        stringstream << fstream.rdbuf();
        src = stringstream.str();
    } 
    else {
        std::cout << "Input file could not be read" << std::endl;
    }

    Scanner scanner(src);
    Parser parser(scanner.tokens);
    ShrExprPtr expression = parser.parse();
    AST ast;
    std::cout << ast.print(expression) << std::endl;
    // print_token_values(scanner.tokens);
}

void from_repl() {
    std::string line;
    std::cout << "tilda | alpha v0.1" << std::endl;

    // Main loop
    while (std::cout << ">> " && std::getline(std::cin, line)) {
        if (line == "q")
            std::exit(0);
        else if (line == "ast") {
            AST::test();
            continue;
        }
        Scanner scanner(line);
        Parser parser(scanner.tokens);
        ShrExprPtr expression = parser.parse();
        AST ast;
        std::cout << ast.print(expression) << std::endl;
        // print_token_values(scanner.tokens);
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