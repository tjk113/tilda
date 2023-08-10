#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <variant>
#include <format>
#include <string>
#include <cmath>
#include <any>

#include "interpreter.hpp"
#include "environment.hpp"
#include "error.hpp"
#include "token.hpp"
#include "tilda.hpp"

void Interpreter::interpret(std::vector<ShrStmtPtr> statements) {
    for (ShrStmtPtr statement : statements)
        execute(statement);
}

std::string Interpreter::to_string(std::any value) {
    if (!value.has_value()) return "void"; // TODO: uh... this
    try {
        std::string text = std::format("{:.4f}", std::any_cast<double>(value));
        if (text.find_last_of('.'))
            text = text.substr(0, text.size() - 2);
        return text;
    }
    catch (std::bad_any_cast) {
        try {
            return std::to_string(std::any_cast<int64_t>(value));
        }
        catch (std::bad_any_cast) {
            try {
                return std::any_cast<bool>(value) ? "true" : "false";
            }
            catch (std::bad_any_cast) {
                return std::any_cast<std::string>(value);
            }
        }
    }
}

std::any Interpreter::evaluate(ShrExprPtr expression) {
    return expression->accept(*this);
}

void Interpreter::execute(ShrStmtPtr statement) {
    statement->accept(*this);
}

void Interpreter::execute_block(std::vector<ShrStmtPtr> statements, std::shared_ptr<Environment> environment) {
    // Make a new environment enclosed by the current environment
    std::shared_ptr<Environment> previous = this->environment;
    // TODO: error handling
    this->environment = environment;
    for (ShrStmtPtr statement : statements) {
        if (!Tilda::had_error && !Tilda::had_runtime_error)
            execute(statement);
    }
    this->environment = previous;
}

std::string Interpreter::get_type(ShrExprPtr expression) {
    std::any value = evaluate(expression);

    const std::type_info& type = value.type();
    if (type == typeid(int64_t))
        return "i64";
    else if (type == typeid(double))
        return "f64";
    else if (type == typeid(std::string))
        return "str";
    else if (type == typeid(bool))
        return "bool";
    else
        return "unknown";
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

// stolen bc lazy: https://github.com/veera-sivarajan/lang0/blob/master/src/Interpreter.cpp#L34
bool Interpreter::get_equality(std::any l_operand, std::any r_operand) {
    if (l_operand.type() == typeid(nullptr) && r_operand.type() == typeid(nullptr)) {
        return true;
    }

    if (l_operand.type() == typeid(nullptr)) {
        return false;
    }

    if (l_operand.type() == typeid(int64_t) && r_operand.type() == typeid(int64_t)) {
        return std::any_cast<int64_t>(l_operand) == std::any_cast<int64_t>(r_operand);
    }

    if (l_operand.type() == typeid(double) && r_operand.type() == typeid(double)) {
        return std::any_cast<double>(l_operand) == std::any_cast<double>(r_operand);
    }

    if (l_operand.type() == typeid(std::string) &&
        r_operand.type() == typeid(std::string)) {
        return std::any_cast<std::string>(l_operand) ==
            std::any_cast<std::string>(r_operand);
    }

    if (l_operand.type() == typeid(bool) && r_operand.type() == typeid(bool)) {
        return std::any_cast<bool>(l_operand) == std::any_cast<bool>(r_operand);
    }

    return false;
}

void Interpreter::throw_error(std::string message) {
    std::cout << std::format("Runtime Error: {}", message) << std::endl;
    Tilda::had_runtime_error = true;
}

bool Interpreter::check_number_operand(TokenType type, std::any operand/*, std::string operand_type*/) {
    if (operand.type() == typeid(int64_t) || operand.type() == typeid(double))
        return false;
    throw_error(std::format("Operand of \"{}\" must be a number.", Token::token_type_names[type]));
    return true;
}

bool Interpreter::check_number_operands(TokenType type, std::any l_operand, std::any r_operand/*,
                                        std::string l_operand_type, std::string r_operand_type*/) {
    if (l_operand.type() == typeid(int64_t) && r_operand.type() == typeid(int64_t)
        || l_operand.type() == typeid(double) && r_operand.type() == typeid(double))
        return false;
    throw_error(std::format("Operands of \"{}\" must be numbers.", Token::token_type_names[type]));
    return true;
}

std::any Interpreter::visit_unary_expression(ShrUnaryExprPtr expression) {
    std::any operand = evaluate(expression->operand);

    switch (expression->type) {
        case NEG:
            if (check_number_operand(expression->type, operand))
                break;
            if (operand.type() == typeid(int64_t))
                return -(std::any_cast<int64_t>(operand));
            else if (operand.type() == typeid(double))
                return -(std::any_cast<double>(operand));
        case L_NOT:
            return !get_truthiness(operand);
        case B_NOT:
            if (operand.type() != typeid(int64_t)) {
                throw_error("Operand of \"~\" must be an integer");
                break;
            }
            else
                return ~(std::any_cast<int64_t>(operand));
        // TODO: how to handle prefix and postfix evaluation cases?
        case INC:
            if (check_number_operand(expression->type, operand))
                break;
            return std::any_cast<double>(operand) + 1;
        case DEC:
            if (check_number_operand(expression->type, operand))
                break;
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

    std::variant<double, std::string, int64_t, bool> l_operand_literal;
    std::variant<double, std::string, int64_t, bool> r_operand_literal;
    // The funniest type deduction you'll ever see
    try {
        try {
            l_operand_literal = std::any_cast<double>(l_operand);
        }
        catch (std::bad_any_cast) {
            try {
                l_operand_literal = std::any_cast<std::string>(l_operand);
            }
            catch (std::bad_any_cast) {
                try {
                    l_operand_literal = std::any_cast<int64_t>(l_operand);
                }
                catch (std::bad_any_cast) {
                    l_operand_literal = std::any_cast<bool>(l_operand);
                }
            }
        }
        try {
            r_operand_literal = std::any_cast<double>(r_operand);
        }
        catch (std::bad_any_cast) {
            try {
                r_operand_literal = std::any_cast<std::string>(r_operand);
            }
            catch (std::bad_any_cast) {
                try {
                    r_operand_literal = std::any_cast<int64_t>(r_operand);
                }
                catch (std::bad_any_cast) {
                    r_operand_literal = std::any_cast<bool>(r_operand);
                }
            }
        }
    }
    catch (std::bad_any_cast) {
        throw_error("Type deduction error.");
        return std::any();
    }
    if (l_operand.type() != r_operand.type()) {
        throw_error("Cannot perform operations on mismatched types.");
        return std::any();
    }

    switch (expression->type) {
        case GREATER:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) > std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) > std::any_cast<double>(r_operand);
        case GREATER_EQ:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) >= std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) >= std::any_cast<double>(r_operand);
        case LESS:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) < std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) < std::any_cast<double>(r_operand);
        case LESS_EQ:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) <= std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) <= std::any_cast<double>(r_operand);
        case EQ:
            return get_equality(l_operand, r_operand);
        case NOT_EQ:
            return !get_equality(l_operand, r_operand);
        case ADD:
            if (l_operand.type() == typeid(double))
                return std::get<double>(l_operand_literal) + std::get<double>(r_operand_literal);
            else if (l_operand.type() == typeid(int64_t))
                return std::get<int64_t>(l_operand_literal) + std::get<int64_t>(r_operand_literal);
            else if (l_operand.type() == typeid(std::string))
                return std::get<std::string>(l_operand_literal) + std::get<std::string>(r_operand_literal);
            // Will throw an error if we reach this
            check_number_operands(expression->type, l_operand, r_operand);
            break;
        case SUB:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) - std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) - std::any_cast<double>(r_operand);
        case MUL:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) * std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) * std::any_cast<double>(r_operand);
        case DIV:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) / std::any_cast<int64_t>(r_operand);
            else
                return std::any_cast<double>(l_operand) / std::any_cast<double>(r_operand);
        case INT_DIV:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) / std::any_cast<int64_t>(r_operand);
            else
                return static_cast<int64_t>(std::any_cast<double>(l_operand)) / static_cast<int64_t>(std::any_cast<double>(r_operand));
        case POW:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return static_cast<int64_t>(std::pow(std::any_cast<int64_t>(l_operand), std::any_cast<int64_t>(r_operand)));
            else
                return std::pow(std::any_cast<double>(l_operand), std::any_cast<double>(r_operand));
        case MOD:
            if (check_number_operands(expression->type, l_operand, r_operand))
                break;
            if (l_operand.type() == typeid(int64_t))
                return std::any_cast<int64_t>(l_operand) % std::any_cast<int64_t>(r_operand);
            else
                return fmod(std::any_cast<double>(l_operand), std::any_cast<double>(r_operand));
    }
    return std::any(); // NULL std::any
}

std::any Interpreter::visit_ternary_expression(ShrTernaryExprPtr expression) {
    // TODO: this
    // bool condition = get_truthiness(evaluate(expression->condition));
    // return condition ? evaluate(expression->l_operand) : evaluate(expression->r_operand);
    ;
}

std::any Interpreter::visit_literal_expression(ShrLiteralExprPtr expression) {
    return expression->value;
}

std::any Interpreter::visit_group_expression(ShrGroupExprPtr expression) {
    return evaluate(expression->expression);
}

std::any Interpreter::visit_variable_expression(ShrVariableExprPtr expression) {
    return environment->get(expression->identifier.lexeme);
}

std::any Interpreter::visit_assign_expression(ShrAssignExprPtr expression) {
    std::any value = evaluate(expression->expression);
    environment->assign(expression->identifier.lexeme, value);
    return value;
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

    try {
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
    }
    catch (std::bad_any_cast) {
        throw_error("Can on only perform bitwise operations on integer types!");
    }
    return std::any(); // NULL std::any
}

void Interpreter::visit_expression_statement(ShrExpressionStmtPtr statement) {
    evaluate(statement->expression);
}

void Interpreter::visit_print_statement(ShrPrintStmtPtr statement) {
    std::cout << to_string(evaluate(statement->expression)) << std::endl;
}

void Interpreter::visit_type_statement(ShrTypeStmtPtr statement) {
    std::cout << std::format("Type: {}", get_type(statement->expression)) << std::endl;
}

void Interpreter::visit_block_statement(ShrBlockStmtPtr statement) {
    execute_block(statement->statements, std::make_shared<Environment>(environment));
}

void Interpreter::visit_declare_statement(ShrDeclareStmtPtr statement) {
    std::any value = std::any();
    if (statement->expression)
        value = evaluate(statement->expression);
    environment->define(statement->identifier.lexeme, value);
}

void Interpreter::visit_assign_statement(ShrAssignStmtPtr statement) {
    ;
}

void Interpreter::visit_if_statement(ShrIfStmtPtr statement) {
    ;
}

void Interpreter::visit_else_statement(ShrElseStmtPtr statement) {
    ;
}

void Interpreter::visit_while_statement(ShrWhileStmtPtr statement) {
    ;
}

void Interpreter::visit_for_statement(ShrForStmtPtr statement) {
    ;
}

void Interpreter::visit_forin_statement(ShrForInStmtPtr statement) {
    ;
}

void Interpreter::visit_switch_statement(ShrSwitchStmtPtr statement) {
    ;
}

void Interpreter::visit_return_statement(ShrReturnStmtPtr statement) {
    ;
}

void Interpreter::visit_struct_statement(ShrStructStmtPtr statement) {
    ;
}