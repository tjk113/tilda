#pragma once

#include <iostream>
#include <optional>
#include <memory>
#include <vector>
#include <any>

#include "environment.hpp"
#include "token.hpp"
#include "types.hpp"

struct Statement;
struct ExpressionStatement;
struct PrintStatement;
struct TypeStatement;
struct BlockStatement;
struct DeclareStatement;
struct IfStatement;
struct WhileStatement;
struct ForStatement;
struct ForInStatement;
struct SwitchStatement;
struct ReturnStatement;
struct StructStatement;

typedef std::shared_ptr<Statement> ShrStmtPtr;
typedef std::shared_ptr<ExpressionStatement> ShrExpressionStmtPtr;
typedef std::shared_ptr<PrintStatement> ShrPrintStmtPtr;
typedef std::shared_ptr<TypeStatement> ShrTypeStmtPtr;
typedef std::shared_ptr<BlockStatement> ShrBlockStmtPtr;
typedef std::shared_ptr<DeclareStatement> ShrDeclareStmtPtr;
typedef std::shared_ptr<IfStatement> ShrIfStmtPtr;
typedef std::shared_ptr<WhileStatement> ShrWhileStmtPtr;
typedef std::shared_ptr<ForStatement> ShrForStmtPtr;
typedef std::shared_ptr<ForInStatement> ShrForInStmtPtr;
typedef std::shared_ptr<SwitchStatement> ShrSwitchStmtPtr;
typedef std::shared_ptr<ReturnStatement> ShrReturnStmtPtr;
typedef std::shared_ptr<StructStatement> ShrStructStmtPtr;

struct StatementVisitor {
    virtual void visit_expression_statement(ShrExpressionStmtPtr statement) = 0;
    virtual void visit_print_statement(ShrPrintStmtPtr statement) = 0;
    virtual void visit_type_statement(ShrTypeStmtPtr statement) = 0;
    virtual void visit_block_statement(ShrBlockStmtPtr statement) = 0;
    virtual void visit_declare_statement(ShrDeclareStmtPtr statement) = 0;
    virtual void visit_if_statement(ShrIfStmtPtr statement) = 0;
    virtual void visit_while_statement(ShrWhileStmtPtr statement) = 0;
    virtual void visit_for_statement(ShrForStmtPtr statement) = 0;
    virtual void visit_forin_statement(ShrForInStmtPtr statement) = 0;
    virtual void visit_switch_statement(ShrSwitchStmtPtr statement) = 0;
    virtual void visit_return_statement(ShrReturnStmtPtr statement) = 0;
    virtual void visit_struct_statement(ShrStructStmtPtr statement) = 0;
    virtual ~StatementVisitor() = default;
};

struct Statement {
    virtual void accept(StatementVisitor& statement_visitor) = 0;
    virtual ~Statement() = default;
};

struct ExpressionStatement : Statement, public std::enable_shared_from_this<ExpressionStatement> {
    ShrExprPtr expression;

    ExpressionStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct PrintStatement : Statement, public std::enable_shared_from_this<PrintStatement> {
    ShrExprPtr expression;

    PrintStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct TypeStatement : Statement, public std::enable_shared_from_this<TypeStatement> {
    ShrExprPtr expression;

    TypeStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct BlockStatement : Statement, public std::enable_shared_from_this<BlockStatement> {
    std::vector<ShrStmtPtr> statements;

    BlockStatement(std::vector<ShrStmtPtr> statements);
    void accept(StatementVisitor& statement_visitor) override;
};

struct DeclareStatement : Statement, public std::enable_shared_from_this<DeclareStatement> {
    Token identifier;
    LiteralType literal_type;
    ShrExprPtr expression;

    DeclareStatement(Token identifier, LiteralType literal_type, ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct IfStatement : Statement, public std::enable_shared_from_this<IfStatement> {
    ShrExprPtr expression;
    ShrStmtPtr then_branch;
    ShrStmtPtr else_branch;

    IfStatement(ShrExprPtr expression, ShrStmtPtr then_branch, ShrStmtPtr else_branch);
    void accept(StatementVisitor& statement_visitor) override;
};

struct WhileStatement : Statement, public std::enable_shared_from_this<WhileStatement> {
    ShrExprPtr expression;
    ShrStmtPtr statements;

    WhileStatement(ShrExprPtr expression, ShrStmtPtr statements);
    void accept(StatementVisitor& statement_visitor) override;
};

struct ForStatement : Statement, public std::enable_shared_from_this<ForStatement> {
    ShrExprPtr expression;

    ForStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct ForInStatement : Statement, public std::enable_shared_from_this<ForInStatement> {
    ShrExprPtr expression;

    ForInStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct SwitchStatement : Statement, public std::enable_shared_from_this<SwitchStatement> {
    ShrExprPtr expression;

    SwitchStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

struct ReturnStatement : Statement, public std::enable_shared_from_this<ReturnStatement> {
    ShrExprPtr expression;

    ReturnStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};

// TODO: ask me later
struct StructStatement : Statement, public std::enable_shared_from_this<StructStatement> {
    ShrExprPtr expression;

    StructStatement(ShrExprPtr expression);
    void accept(StatementVisitor& statement_visitor) override;
};