#pragma once

#include <string>
#include <map>

enum class LiteralType {
    U8, I8,
    U16, I16,
    U32, I32, INT,
    U64, I64,
    F32, F64,
    STR,
    BOOL,
    VOID
};

// std::map<std::string, LiteralType> string_to_literal_type = {
//     {"u8", LiteralType::U8},
//     {"i8", LiteralType::I8},
//     {"u16", LiteralType::U16},
//     {"i16", LiteralType::I16},
//     {"u32", LiteralType::U32},
//     {"i32", LiteralType::I32},
//     {"int", LiteralType::INT},
//     {"u64", LiteralType::U64},
//     {"i64", LiteralType::I64},
//     {"f32", LiteralType::F32},
//     {"f64", LiteralType::F64},
//     {"str", LiteralType::STR},
//     {"bool", LiteralType::BOOL},
//     {"void", LiteralType::VOID},
// };