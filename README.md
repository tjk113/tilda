# tilda
a (very wip) C-like general purpose programming language

implementation informed by [Crafting Interpreters](https://craftinginterpreters.com/)

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
  Used in `for i in a..b` loops.
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
#### ternary
- T_IF `?`
- T_ELSE `:`  
  `a ? b : c`
### bitwise
- OR  `a | b`
- AND `a & b`
- XOR `a ^ b`
- NOT `~a`
- LSHFT `a << b`
- RSHFT `a >> b`
- CHK `a@b`  
  Returns `true` if the bit at the specified offset is set

## built-in functions
- `print()` / `println()`  
  Prints provided value. It will attempt to print values of any type, and throw an error if it can't.

## scoping
scopes are declared with curly braces `{}`. single-line scopes can be declared with a newline, where the scope begins and ends on the following line.