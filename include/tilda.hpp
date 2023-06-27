#pragma once

#include <vector>

#include "expression.hpp"

struct TildaEnvironment {
    std::vector<ShrExprPtr> expressions;
    TildaEnvironment(std::vector<ShrExprPtr> expressions);
    bool run();
};