#ifndef A_CODE_INSTRUCTIONS_HPP
#define A_CODE_INSTRUCTIONS_HPP

namespace A
{
  enum class CodeInstruction : unsigned{
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

    NAMESPACE,
  };
}

#endif // A_CODE_INSTRUCTIONS_HPP

/*
let int a = 3
if a then
a = a + 3
else 
a = a - 3
end if


{
0   PROGRAM_START,
1   DECLARE,
2   TYPE_OF,
3   1,
4   VALUE_OF,
5   LITERAL,
6   0, // address of literal; its index in "_literals"
7   TEST,
8   STORED, // address of data stored in memory
9   0,
10  12, #IF TRUE
11  19, #IF FALSE
12  ASSIGN,
13  0
14  ADD,
15  STORED,
16  0,
17  LITERAL,
18  0
19  ASSIGN,
20  0,
21  SUBTRACT,
22  STORED,
23  0,
24  LITERAL,
25  0
26  PROGRAM_END
}

 */
