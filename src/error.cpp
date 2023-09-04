#include <iostream>
#include <format>
#include <string>

#include "error.hpp"
#include "tilda.hpp"
#include "token.hpp"

void Error::report(int line, std::string where, std::string message) {
    Tilda::had_error = true;
    throw std::format("[line {}] Error {}: {}", line, where, message);
}