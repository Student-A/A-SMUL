#ifndef A_CODE_INSTRUCTIONS_HPP
#define A_CODE_INSTRUCTIONS_HPP

namespace A
{
  enum CodeInstruction{
    NONE,
    PROGRAM_START,
    PROGRAM_END,
    EXIT,

    NOP,
    
    DECLARE,

    TYPE_OF,
    CONSTANT_OF,
    VALUE_OF,

    ASSIGN,
    
    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,

    CALL, // after call comes #params, then params
    RETURN,
    RETURN_VALUE_OF,
    
    JUMP,
    TEST,

    LITERAL,
    REFERENCE,
    STORED,
    NUMBER_OF,
    TEXT_OF,
    LIST_OF,
    
  };
}

#endif // A_CODE_INSTRUCTIONS_HPP
