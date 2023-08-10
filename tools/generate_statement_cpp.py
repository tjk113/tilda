import re

type_names = ['TokenType', 'Token', 'LiteralType', 'const', 'std::any', '=', 'ShrStmtPtr', 'ShrExprPtr', 'std::vector<ShrStmtPtr>', 'bool']

stmt_type_pattern = re.compile(r'struct (?P<type>[A-Za-z0-9]+) : Statement, public std::enable_shared_from_this<[A-Za-z]+> {')
stmt_constructor_pattern = r'\((?P<args>[\w,\*;:<>_ ]+)\);'

def generate_stmt_dict() -> dict[str: str]:
    stmt_types = {}
    stmt_type = None
    with open('include\\statement.hpp', 'r') as header_file:
        for line in header_file:
            if 'struct' in line and 'StatementVisitor' not in line \
                and line.count('Statement') > 2:
                    stmt_type = stmt_type_pattern.search(line).group('type')
                    continue
            if stmt_type and stmt_type in line and '//' not in line:
                pattern = f'{stmt_type}{stmt_constructor_pattern}'
                stmt_args = re.search(pattern, line).group('args')
                stmt_types[stmt_type] = stmt_args
    return stmt_types

def get_parameter_names(stmt_args: str) -> list[str]:
    toks = stmt_args.replace(',', '').split(' ')
    return [param_name for param_name in toks if param_name not in type_names]

def write_statement_cpp(stmt_types: dict) -> None:
    with open('src\\statement.cpp', 'w+') as cpp_file:
        cpp_file.write('#include "expression.hpp"\n#include "statement.hpp"\n#include <any>\n')
        for stmt_type, stmt_args in stmt_types.items():
            stmt_type_name = stmt_type[:-9].lower()
            param_names = get_parameter_names(stmt_args)
            param_assignments = ', '.join(
                [f'{param_name}({param_name})'
                 for param_name in param_names]) + ' {}'

            cpp_file.write(f'''
{stmt_type}::{stmt_type}({stmt_args}) :
    {param_assignments}

void {stmt_type}::accept(StatementVisitor& visitor) {{
    return visitor.visit_{stmt_type_name}_statement(shared_from_this());
}}
''')
            
write_statement_cpp(generate_stmt_dict())