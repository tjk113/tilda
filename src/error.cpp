#include <iostream>
#include <format>
#include <string>

#include "error.hpp"
#include "tilda.hpp"
#include "token.hpp"

Error::Error(Token token, std::string message) {
    ;
}

void Error::report(int line, std::string where, std::string message) {
    std::cout << std::format("[line {}] Error {}: {}", line, where, message) << std::endl;
    had_error = true;
}