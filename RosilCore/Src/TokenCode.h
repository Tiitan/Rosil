#pragma once

#include <unordered_map>

enum TokenCode
{
    Label,              // [aA-zZ]_[0-9]
    Text,               // ""
    IntValue,           // start with [0-9] 
    FloatValue,         // start with [0-9] contains '.'
    TrueValue,
    FalseValue,

    // Arithmetic operator
    Add,                // +
    Sub,                // -
    Divide,             // /
    Multiply,           // *
    Modulo,             // %
    
    // Boolean operator
    And,                // &&
    Or,                 // ||
    Xor,                // ^
    Inferior,           // <
    Superior,           // >
    InferiorOrEquals,   // <=
    SuperiorOrEquals,   // >=
    Equals,             // ==
    NotEquals,          // !=
    Invert,             // !

    // Assign    
    Assign,             // =
    AddAssign,          // +=
    SubAssign,          // -=
    DivAssign,          // /=
    MultAssign,         // *=
    ModAssign,          // %=
    Increment,          // ++
    Decrement,          // --

    // Separators
    Accessor,           // .
    ParameterSeparator, // ,
    StatementSeparator, // ;
    OpenContext,        // :
    CloseContext,       // one for each removed tab on next line (or one for inlined context)
    OpenParenthesis,    // (
    CloseParenthesis,   // )
    OpenBracket,        // {
    CloseBracket,       // }
    OpenBrace,          // [
    CloseBrace,         // ]

    // Control
    If,
    Else,
    ElseIf,             // Else + If
    While,
    For,
    Switch,
    Case,
    Get,
    Set,

    // Type
    BoolType,
    IntType,
    FloatType,
    StringType,
    ClassType,
    StructType,
    EnumType,
    ListType,
    DictType,
    Import,

    // Access
    Public,
    Private,
    Static,
    Const,
};
