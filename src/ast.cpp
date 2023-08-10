#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <any>

#include "expression.hpp"
#include "token.hpp"
#include "common.hpp"
#include "ast.hpp"

std::string AST::print(ShrExprPtr expression) {
    return std::any_cast<std::string>(expression->accept(*this));
}

// TODO: why is this getting passed a LiteralExpression*???
template<class... E>
std::string AST::parenthesize(std::string label, E... expressions) {
    std::ostringstream buf;

    buf << "(" << label;
    ((buf << " " << print(expressions)), ...);
    buf << ")";

    return buf.str();
}

// void AST::test() {
//     // literal strings must *always* be std::string, and not const char*!
//     ShrTernaryExprPtr ternary_expression = std::make_shared<TernaryExpression>
//     (TERN,
//         std::make_shared<LogicalExpression>(
//             EQ,
//             std::make_shared<LiteralExpression>(
//                 STR, std::string("Jason")
//             ),
//             std::make_shared<LiteralExpression>(
//                 STR, std::string("Mark")
//             )
//         ),
//         std::make_shared<LiteralExpression>(
//             NUM, 3
//         ),
//         std::make_shared<GroupExpression>(
//             std::make_shared<UnaryExpression>(
//                 NEG, std::make_shared<LiteralExpression>(
//                     NUM, 5
//                 ), false
//             )
//         )
//     );

//     AST ast;
//     std::cout << ast.print(ternary_expression) << std::endl;
// }

std::any AST::visit_unary_expression(ShrUnaryExprPtr expression) {
    ShrLiteralExprPtr pre_or_postfix = std::make_shared<LiteralExpression>(
        STR, std::string(expression->postfix ? "POST" : "PRE"));
    return parenthesize(Token::token_type_names[expression->type], pre_or_postfix, expression->operand);
}

std::any AST::visit_binary_expression(ShrBinaryExprPtr expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}

std::any AST::visit_ternary_expression(ShrTernaryExprPtr expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->condition, expression->l_operand, expression->r_operand);
}

std::any AST::visit_literal_expression(ShrLiteralExprPtr expression) {
    auto value = expression->value;
    auto type = expression->type;
    switch (expression->type) {
        case STR:
        case TYPE:
        case IDENTIFIER:
            return std::any_cast<std::string>(expression->value);
        /* TODO: for different internal number representations,
        more cases will have to be added to this switch block */
        case NUM:
            // never actually the case atm, bc all nums are lexed as doubles
            try {
                return std::to_string(std::any_cast<int>(expression->value));
            }
            catch (std::bad_any_cast) {
                return std::to_string(std::any_cast<double>(expression->value));
            }
        case TRUE:
        case FALSE:
            return std::string(std::any_cast<bool>(expression->value) ? "TRUE" : "FALSE");
        default:
            return std::string("unknown literal type");
    }
}

std::any AST::visit_group_expression(ShrGroupExprPtr expression) {
    return parenthesize("GROUP", expression->expression);
}

std::any AST::visit_assign_expression(ShrAssignExprPtr expression) {
    ShrLiteralExprPtr identifier = std::make_shared<LiteralExpression>(
        IDENTIFIER, expression->identifier.lexeme);
    return parenthesize("ASSIGN", identifier, expression->expression);
}

std::any AST::visit_range_expression(ShrRangeExprPtr expression) {
    return parenthesize("RANGE", expression->l_operand, expression->r_operand);
}

std::any AST::visit_access_expression(ShrAccessExprPtr expression) {
    return parenthesize("ACCESS", expression->l_operand, expression->r_operand);
}

std::any AST::visit_call_expression(ShrCallExprPtr expression) {
    // TODO: support unpacking vector into this function call so all arguments can be printed
    ShrLiteralExprPtr function_name = std::make_shared<LiteralExpression>(
        IDENTIFIER, expression->function_name.lexeme);
    return parenthesize("CALL", function_name, expression->arguments[0]);
}

std::any AST::visit_logical_expression(ShrLogicalExprPtr expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}

std::any AST::visit_bitwise_expression(ShrBitwiseExprPtr expression) {
    return parenthesize(Token::token_type_names[expression->type], expression->l_operand, expression->r_operand);
}