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

struct StringToLiteralType {
    static std::map<std::string, LiteralType> map;
};