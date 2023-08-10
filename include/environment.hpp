#pragma once

#include <memory>
#include <string>
#include <any>
#include <map>

class Environment : std::enable_shared_from_this<Environment> {
    std::shared_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;
    void throw_error(std::string message);
public:
    Environment();
    Environment(std::shared_ptr<Environment> enclosing);
    void define(std::string identifier, std::any value);
    void assign(std::string identifier, std::any value);
    std::any get(std::string identifier);
};