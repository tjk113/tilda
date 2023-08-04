#include <stdint.h>
#include <variant>
#include <string>
#include <cmath>
#include <any>

#include "interpreter.hpp"

Interpreter::Interpreter() {
    ;
}

std::any Interpreter::evaluate(ShrExprPtr expression) {
    return expression->accept(*this);
}

bool Interpreter::get_truthiness(std::any operand) {
    if (!operand.has_value())
        return false;
    try {
        return std::any_cast<bool>(operand);
    }
    catch (std::bad_any_cast) {
        try {
            return std::any_cast<double>(operand) > 0 ? true : false;
        }
        catch (std::bad_any_cast) {
            try {
                std::string str = std::any_cast<std::string>(operand);
                return str != "";
            }
            catch (std::bad_any_cast) {
                ;
            }
        }
    }
    return true;
}

std::any Interpreter::visit_unary_expression(ShrUnaryExprPtr expression) {
    std::any operand = evaluate(expression->operand);
    auto opr_type = typeid(operand).name();

    switch (expression->type) {
        case NEG:
            return -(std::any_cast<double>(operand));
        case L_NOT:
            return !get_truthiness(operand);
        case B_NOT:
            return ~(static_cast<int64_t>(std::any_cast<double>(operand))); // oh, this is bad
        // TODO: how to handle prefix and postfix evaluation cases?
        case INC:
            return std::any_cast<double>(operand) + 1;
        case DEC:
            return std::any_cast<double>(operand) - 1;
    }
    // Unreachable
    return std::any(); // NULL std::any
}

std::any Interpreter::visit_binary_expression(ShrBinaryExprPtr expression) {
    std::any l_operand = evaluate(expression->l_operand);
    std::any r_operand = evaluate(expression->r_operand);

    /* TODO: uh. figure out when and how to actually handle type
    resolution */

    std::variant<double, std::string> l_operand_literal;
    std::variant<double, std::string> r_operand_literal;
    // The funniest type deduction you'll ever see
    try {
        try {
            l_operand_literal = std::any_cast<double>(l_operand);
        }
        catch (std::bad_any_cast) {
            l_operand_literal = std::any_cast<std::string>(l_operand);
        }
        try {
            r_operand_literal = std::any_cast<double>(r_operand);
        }
        catch (std::bad_any_cast) {
            r_operand_literal = std::any_cast<std::string>(r_operand);
        }
    }
    catch (std::bad_any_cast) {
        std::cout << "Type deduction error!" << std::endl;
        return std::any();
    }
    if (typeid(l_operand_literal) != typeid(r_operand_literal)) {
        std::cout << "Cannot perform operations on mismatched types!" << std::endl;
        return std::any();
    }

    try {
        switch (expression->type) {
            case ADD:
                if (typeid(l_operand_literal) == typeid(double))
                    return std::get<double>(l_operand_literal) + std::get<double>(r_operand_literal);
                else if (typeid(l_operand_literal) == typeid(std::string))
                    return std::get<std::string>(l_operand_literal) + std::get<std::string>(r_operand_literal);
            case SUB:
                return std::any_cast<double>(l_operand) - std::any_cast<double>(r_operand);
            case MUL:
                return std::any_cast<double>(l_operand) * std::any_cast<double>(r_operand);
            case DIV:
                return std::any_cast<double>(l_operand) / std::any_cast<double>(r_operand);
            case INT_DIV:
                return std::any_cast<double>(l_operand) - std::any_cast<double>(r_operand);
            case POW:
                return std::pow(std::any_cast<double>(l_operand), std::any_cast<double>(r_operand));
            case MOD:
                return fmod(std::any_cast<double>(l_operand), std::any_cast<double>(r_operand));
        }
    }
    catch (std::bad_any_cast) {
        std::cout << "Runtime type error!" << std::endl;
        return std::any();
    }
}

std::any Interpreter::visit_ternary_expression(ShrTernaryExprPtr expression) {
    ;
}

std::any Interpreter::visit_literal_expression(ShrLiteralExprPtr expression) {
    return expression->value;
}

std::any Interpreter::visit_group_expression(ShrGroupExprPtr expression) {
    return evaluate(expression->expression);
}

std::any Interpreter::visit_assign_expression(ShrAssignExprPtr expression) {
    ;
}

std::any Interpreter::visit_range_expression(ShrRangeExprPtr expression) {
    ;
}

std::any Interpreter::visit_access_expression(ShrAccessExprPtr expression) {
    ;
}

std::any Interpreter::visit_call_expression(ShrCallExprPtr expression) {
    ;
}

std::any Interpreter::visit_logical_expression(ShrLogicalExprPtr expression) {
    ;
}

std::any Interpreter::visit_bitwise_expression(ShrBitwiseExprPtr expression) {
    std::any l_operand = evaluate(expression->l_operand);
    std::any r_operand = evaluate(expression->r_operand);

    // try {
        switch (expression->type) {
            case B_OR:
                return std::any_cast<int64_t>(l_operand) |
                    std::any_cast<int64_t>(r_operand);
            case B_AND:
                return std::any_cast<int64_t>(l_operand) &
                    std::any_cast<int64_t>(r_operand);
            case B_XOR:
                return std::any_cast<int64_t>(l_operand) ^
                    std::any_cast<int64_t>(r_operand);
            case LSHFT:
                return std::any_cast<int64_t>(l_operand) <<
                    std::any_cast<int64_t>(r_operand);
            case RSHFT:
                return std::any_cast<int64_t>(l_operand) >>
                    std::any_cast<int64_t>(r_operand);
            case CHK:
                #define CHECK_BIT(var, offset) (var >> offset) & 1
                return CHECK_BIT(std::any_cast<int64_t>(l_operand),
                    std::any_cast<int64_t>(r_operand));
                #undef CHECK_BIT
        }
    // }
    // catch (std::bad_any_cast) {
    //     std::cout << "Can on only perform bitwise operations on integer types!" << std::endl;
    //     return std::any();
    // }
}