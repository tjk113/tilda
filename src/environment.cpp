#include <iostream>
#include <memory>
#include <format>
#include <string>
#include <any>
#include <map>

#include "environment.hpp"
#include "tilda.hpp"

Environment::Environment() {
    enclosing = nullptr;
}

Environment::Environment(std::shared_ptr<Environment> enclosing) :
    enclosing(enclosing) {}

void Environment::throw_error(std::string message) {
    Tilda::had_runtime_error = true;
    throw message;
}

void Environment::define(std::string identifier, std::any value) {
    values.insert({identifier, value});
}

void Environment::assign(std::string identifier, std::any value) {
    if (!values.count(identifier)) {
        if (enclosing != nullptr)
            enclosing->assign(identifier, value);
        else
            throw_error(std::format("Undefined variable: \"{}\".", identifier));

        return;
    }

    values[identifier] = value;
}

std::any Environment::get(std::string identifier) {
    // Check to make sure variable is defined
    if (!values.count(identifier)) {
        if (enclosing != nullptr)
            return enclosing->get(identifier);
        else {
            throw_error(std::format("Undefined variable: \"{}\".", identifier));
            return std::any(); // NULL std::any
        }
    }

    return values[identifier];
}