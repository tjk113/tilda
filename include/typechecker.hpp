#pragma once

#include <vector>

#include "statement.hpp"
#include "token.hpp"

class Typechecker {

public:
    Typechecker(std::vector<ShrStmtPtr> statements);
};