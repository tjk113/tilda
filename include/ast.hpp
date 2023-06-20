#pragma once

#include "token.hpp"

struct AST {
    struct Node {
        TokenType type;
        Node* l_child;
        Node* r_child;
    };
    
};