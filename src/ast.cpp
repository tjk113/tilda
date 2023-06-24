#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <any>

#include "expression.hpp"
#include "token.hpp"
#include "ast.hpp"

// AST::AST(std::vector<Token> toks) {
//     ;
// }

std::string AST::print(const Expression* expression) {
    return std::any_cast<std::string>(expression->accept(*this));
}

template<class... E>
std::string AST::parenthesize(std::string label, E... expressions) {
    std::ostringstream buf;

    buf << "(" << label;
    ((buf << " " << print(expressions)), ...);
    buf << ")";

    return buf.str();
}

void AST::test() {
    // literal strings must *always* be std::string, and not const char*!
    LiteralExpression num1 = LiteralExpression(NUM, 3);
    LiteralExpression num2 = LiteralExpression(NUM, 1);
    LiteralExpression num3 = LiteralExpression(NUM, 42069);
    UnaryExpression unary_expression = UnaryExpression(NEG, &num3);
    GroupExpression group_expression = GroupExpression(&unary_expression);
    BitwiseExpression bitwise_expression = BitwiseExpression(B_XOR, &num1, &num2);

    BinaryExpression binary_expression(ADD, &num1, &group_expression);

    AST ast;
    std::cout << ast.print(&bitwise_expression) << std::endl;
}

std::any AST::visit_unary_expression(const UnaryExpression* expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->operand);
}

std::any AST::visit_binary_expression(const BinaryExpression* expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}

std::any AST::visit_literal_expression(const LiteralExpression* expression) {
    switch (expression->type) {
        case STR:
        case TYPE:
        case IDENTIFIER:
            return std::any_cast<std::string>(expression->value);
        /* TODO: for different internal number representations,
        more cases will have to be added to this switch block */
        case NUM:
            try {
                return std::to_string(std::any_cast<int>(expression->value));
            }
            catch (std::bad_any_cast) {
                return std::to_string(std::any_cast<double>(expression->value));
            }
        default:
            return std::string("unknown literal type");
    }
}

std::any AST::visit_group_expression(const GroupExpression* expression) {
    return parenthesize("GROUP", expression->expression);
}

std::any AST::visit_assign_expression(const AssignExpression* expression) {
    LiteralExpression identifier(IDENTIFIER, expression->identifier.lexeme);
    return parenthesize("ASSIGN", &identifier, expression->expression);
}

std::any AST::visit_range_expression(const RangeExpression* expression) {
    return parenthesize("RANGE", expression->l_operand, expression->r_operand);
}

std::any AST::visit_access_expression(const AccessExpression* expression) {
    return parenthesize("ACCESS", expression->l_operand, expression->r_operand);
}

std::any AST::visit_call_expression(const CallExpression* expression) {
    // TODO: support unpacking vector into this function call so all arguments can be printed
    LiteralExpression function_name(IDENTIFIER, expression->function_name.lexeme);
    return parenthesize("CALL", &function_name, expression->arguments[0]);
}

std::any AST::visit_logical_expression(const LogicalExpression* expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}

std::any AST::visit_bitwise_expression(const BitwiseExpression* expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}