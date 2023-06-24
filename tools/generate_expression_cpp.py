import re

type_names = ['TokenType', 'Token', 'const', 'Expression*', 'Expression', 'std::any', 'std::vector<const', 'std::vector<', 'Expression*>']

expr_type_pattern = re.compile(r'struct (?P<type>[A-Za-z0-9]+) : Expression {')
expr_constructor_pattern = r'\((?P<args>[\w,\*;:<> ]+)\);'

def generate_expr_dict() -> dict[str: str]:
    expr_types = {}
    expr_type = None
    with open('include\\expression.hpp', 'r') as header_file:
        for line in header_file:
            if 'struct' in line and 'ExpressionVisitor' not in line \
                and not line.count('Expression') == 1:
                    expr_type = expr_type_pattern.match(line).group('type')
                    continue
            if expr_type and expr_type in line:
                pattern = f'{expr_type}{expr_constructor_pattern}'
                expr_args = re.search(pattern, line).group('args')
                expr_types[expr_type] = expr_args
    return expr_types

def get_parameter_names(expr_args: str) -> list[str]:
    toks = expr_args.replace(',', '').split(' ')
    return [param_name for param_name in toks if param_name not in type_names]

def write_expression_cpp(expr_types: dict) -> None:
    with open('src\\expression.cpp', 'w+') as cpp_file:
        cpp_file.write('#include "expression.hpp"\n#include <any>\n')
        for expr_type, expr_args in expr_types.items():
            expr_type_name = expr_type[:-10].lower()
            param_names = get_parameter_names(expr_args)
            param_assignments = ', '.join(
                [f'{param_name}({param_name})'
                 for param_name in param_names]) + ' {}'

            cpp_file.write(f'''
{expr_type}::{expr_type}({expr_args}) :
    {param_assignments}

std::any {expr_type}::accept(ExpressionVisitor<std::any>& visitor) const {{
    return visitor.visit_{expr_type_name}_expression(this);
}}
''')
            
write_expression_cpp(generate_expr_dict())