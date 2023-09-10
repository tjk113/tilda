# tilda
a (very wip) C-like general purpose programming language

implementation informed by [Crafting Interpreters](https://craftinginterpreters.com/)

## formal grammar
```
program               -> declaration_statement* END_TOKEN ;

declaration_statement -> variable_declaration NEWLINE
                       | statement NEWLINE ;

variable_declaration  -> "const"? TYPE IDENTIFIER ( "=" expression )?
                       | ( "let" | "const" ) IDENTIFIER ( "=" expression )? ;

statement             -> expression_statement
                       | for_statement
                       | if_statement
                       | print_statement
                       | while_statement
                       | block ;

for_statement         -> "for" "(" ( variable_declaration | expression_statement | ";" )
                         expression? ";"
                         expression? ";" ")" statement ;

while_statement       -> "while" "(" expression ")" statement ;

if_statement          -> "if" "(" expression ")" block
                         ( "elif" "(" expression ")" block )?
                         ( "else" block )?

block                 -> "{" declaration_statement* "}" ;

print_statement       -> "print" expression ;

expression_statement  -> expression ;

expression            -> assignment ;

assignment            -> IDENTIFIER ( "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "|=" | "&=" | "^=" ) assignment
                       | logical_or ;

logical_or            -> logical_and ( "||" logical_and )* ;

logical_and           -> equality ( "&&" equality )* ;

ternary               -> equality "?" equality ":" ( expression )* ;

equality              -> comparison ( ( "!=" | "==" ) ) comparison )* ;

comparison            -> bitwise ( ( ">" | ">=" | "<" | "<=" ) bitwise )* ;

bitwise               -> term ( ( "|" | "&" | "^" | "@" | "<<" | ">>" ) ( term | bitwise ) )* ;

term                  -> factor ( ( "-" | "+" ) factor )* ;

factor                -> unary ( ( "/" | "*" | "**" | "%" ) unary )* ;

unary                 -> ( "!" | "-" | "~" | "++" | "--" )? unary ( "++" | "--" )?  
                       | primary ;

primary               -> NUMBER | STRING | TRUE | FALSE
                       | IDENTIFIER
                       | "(" expression ")" ;
```

## keywords
- `if`
- `elif`
- `else`
- `while`
- `for`
- `break`
- `continue`
- `in`
- `switch`
- `return`
- `true`
- `false`
- `let`
- `const`
- `fn`
- `struct`

## types
- `u8 / i8`
- `u16 / i16`
- `u32 / i32 (int)`
- `u64 / i64`
- `f32`
- `f64`
- `str`
- `void`
- `bool`
- Pointers (`[TYPE]*`)

## operators
### language
- ASSIGN `a = b`
- CALL `a([b...])`
- RANGE `a..b`
- ACCESS `a.b`
- DEREF_ACCESS `a->b`
- DEREF `*a`
### mathematical
- ADD `a + b`
- SUB `a - b`
- MUL `a * b`
- DIV `a / b`
- POW `a**b`
- NEG `-a`
- MOD `a % b`
- ADD_EQ `+=`
- SUB_EQ `-=`
- MUL_EQ `*=`
- DIV_EQ `/=`
- POW_EQ `**=`
- INC `a++` / `++a`
- DEC `a--` / `--a`
### logical
- LESS `a < b`
- GREATER `a > b`
- LESS_EQ `a <= b`
- GREATER_EQ `a >= b`
- NOT_EQ `a != b`
- EQ `a == b`
- OR `a || b`
- AND `a && b`
- XOR `a ^^ b`
- NOT `!a`  
- TERN `?:`  
  `a ? b : c`
### bitwise
- OR  `a | b`
- AND `a & b`
- XOR `a ^ b`
- NOT `~a`
- LSHFT `a << b`
- RSHFT `a >> b`
- CHK `a@b`  
  Returns the bit at position `b` in `a` (0 = least significant bit)

## number representations
- Decimal `1`
- Floating Point `1.0`
- Binary `0b00000001`
- Hexadecimal `0x01`

## built-in functions
- `print()` / `println()`  
  Prints provided value. It will attempt to print values of any type, and throw an error if it can't.
- `hex()` / `bin()`  
  Returns a string containing the respective hexadecimal or binary representation of the provided number.
- `type()`
  Returns a string containing the type of the provided value.

## scoping
scopes are declared with curly braces `{}`. single-line scopes can be declared within a single line, or with a newline, where the scope begins and ends on the following line.