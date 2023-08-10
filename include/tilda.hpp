#pragma once

#include <vector>

#include "expression.hpp"

struct Tilda {
    static bool had_error;
    static bool had_runtime_error;

    static bool run();
};