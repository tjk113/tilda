#include "expression.hpp"
#include "statement.hpp"
#include <any>

ExpressionStatement::ExpressionStatement(ShrExprPtr expression) :
    expression(expression) {}

void ExpressionStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_expression_statement(shared_from_this());
}

PrintStatement::PrintStatement(ShrExprPtr expression) :
    expression(expression) {}

void PrintStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_print_statement(shared_from_this());
}

TypeStatement::TypeStatement(ShrExprPtr expression) :
    expression(expression) {}

void TypeStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_type_statement(shared_from_this());
}

BlockStatement::BlockStatement(std::vector<ShrStmtPtr> statements) :
    statements(statements) {}

void BlockStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_block_statement(shared_from_this());
}

DeclareStatement::DeclareStatement(Token identifier, LiteralType literal_type, ShrExprPtr expression) :
    identifier(identifier), literal_type(literal_type), expression(expression) {}

void DeclareStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_declare_statement(shared_from_this());
}

IfStatement::IfStatement(ShrExprPtr expression, ShrStmtPtr then_branch, ShrStmtPtr else_branch) :
    expression(expression), then_branch(then_branch), else_branch(else_branch) {}

void IfStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_if_statement(shared_from_this());
}

WhileStatement::WhileStatement(ShrExprPtr expression, ShrStmtPtr statements) :
    expression(expression), statements(statements) {}

void WhileStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_while_statement(shared_from_this());
}

ForStatement::ForStatement(ShrExprPtr expression) :
    expression(expression) {}

void ForStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_for_statement(shared_from_this());
}

ForInStatement::ForInStatement(ShrExprPtr expression) :
    expression(expression) {}

void ForInStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_forin_statement(shared_from_this());
}

SwitchStatement::SwitchStatement(ShrExprPtr expression) :
    expression(expression) {}

void SwitchStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_switch_statement(shared_from_this());
}

ReturnStatement::ReturnStatement(ShrExprPtr expression) :
    expression(expression) {}

void ReturnStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_return_statement(shared_from_this());
}

StructStatement::StructStatement(ShrExprPtr expression) :
    expression(expression) {}

void StructStatement::accept(StatementVisitor& visitor) {
    return visitor.visit_struct_statement(shared_from_this());
}
