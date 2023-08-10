#include <string>
#include <map>

#include "types.hpp"

std::map<std::string, LiteralType> StringToLiteralType::map = {
    {"u8", LiteralType::U8},
    {"i8", LiteralType::I8},
    {"u16", LiteralType::U16},
    {"i16", LiteralType::I16},
    {"u32", LiteralType::U32},
    {"i32", LiteralType::I32},
    {"int", LiteralType::INT},
    {"u64", LiteralType::U64},
    {"i64", LiteralType::I64},
    {"f32", LiteralType::F32},
    {"f64", LiteralType::F64},
    {"str", LiteralType::STR},
    {"bool", LiteralType::BOOL},
    {"void", LiteralType::VOID},
};