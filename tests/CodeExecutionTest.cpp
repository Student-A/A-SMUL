#include <iostream>
#include "CodeExecutable.hpp"

int main()
{
  std::variant<
		 double,
		 bool> value;
  double howdy = 123.0;
  value = howdy;
  /*
    let number a = 3
    if a > 3 then
    a = a + 3
    else 
    a = a - 3
    end
  */
  
  int three = 3;
  
  std::vector<A::Value> literals = {A::Value(&three, A::ValueType::NUMBER)};
  
  std::vector<A::CodeInstruction> instructions =
    {
      A::CodeInstruction::PROGRAM_START, //address of command is its index. so address of this line is 0
      A::CodeInstruction::DECLARE,
      A::CodeInstruction::TYPE_OF,
      1,
      A::CodeInstruction::VALUE_OF,
      A::CodeInstruction::LITERAL,
      0, // address of literal; its index in "_literals"
      A::CodeInstruction::TEST,
      A::CodeInstruction::EQUALS,
      A::CodeInstruction::STORED, 
      0,// address of data stored in memory
      A::CodeInstruction::LITERAL,
      0,
      15, // if True jump to line 15
      22, // if False jump to line 22
      A::CodeInstruction::ASSIGN,
      0,
      A::CodeInstruction::ADD,
      A::CodeInstruction::STORED,
      0,
      A::CodeInstruction::LITERAL,
      0,
      A::CodeInstruction::ASSIGN,
      0,
      A::CodeInstruction::SUBTRACT,
      A::CodeInstruction::STORED,
      0,
      A::CodeInstruction::LITERAL,
      0,
      A::CodeInstruction::PROGRAM_END
    };

  A::CodeExecutable exe(instructions, literals):
  
  return 0;
}
