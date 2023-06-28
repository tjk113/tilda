# tilda
a (very wip) C-like general purpose programming language

implementation informed by [Crafting Interpreters](https://craftinginterpreters.com/)

## formal grammar
<code>expression&nbsp;-> ternary ;

ternary&nbsp;&nbsp;&nbsp;&nbsp;-> equality "?" equality ":" ( expression )* ;

equality&nbsp;&nbsp;&nbsp;-> comparison ( ( "!=" | "==" ) ) comparison )* ;

comparison -> bitwise ( ( ">" | ">=" | "<" | "<=" ) bitwise )* ;

bitwise&nbsp;&nbsp;&nbsp;&nbsp;-> term ( ( "|" | "&" | "^" | "@" | "<<" | ">>" ) term )* ;

term&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> factor ( ( "-" | "+" ) factor )* ;

factor&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> unary ( ( "/" | "\*" ) unary )\* ;

unary&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> ( "!" | "-" | "~" ) unary  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| primary ;

primary&nbsp;&nbsp;&nbsp;&nbsp;-> NUMBER | STRING | TRUE | FALSE  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| "(" expression ")" ;</code>

## keywords
- `if`
- `else`
- `while`
- `for`
- `break`
- `continue`
- `in`
- `goto`
- `switch`
- `return`
- `true`
- `false`
- `let`
- `const`
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
- DEREF `*a`
### mathematical
- ADD `a + b`
- SUB `a - b`
- MUL `a * b`
- DIV `a / b`
- INT_DIV `a \ b`
- POW `a**b`
- NEG `-a`
- MOD `a % b`
- INC `a++`
- DEC `b--`
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

## built-in functions
- `print()` / `println()`  
  Prints provided value. It will attempt to print values of any type, and throw an error if it can't.

## scoping
scopes are declared with curly braces `{}`. single-line scopes can be declared with a newline, where the scope begins and ends on the following line.