#ifndef A_CODE_EXECUTABLE_HPP
#define A_CODE_EXECUTABLE_HPP

#include "CodeInstructions.hpp"
#include "Memory.hpp"
#include "Value.hpp"

namespace A
{
  class CodeExecutable
  {
  public:
    CodeExecutable(Memory *memory, std::vector<CodeInstruction> instructions, std::vector<Value> literals)
    {
      _instructions = instructions;
    }

    ~CodeExecutable();
    
    void execute()
    {
      CodeInstruction inst = _instructions[_currentInstructionAddress];
      if (inst == PROGRAM_START)
    }

    void execute_cycle()
    {
      CodeInstruction inst = _instructions[_currentInstructionAddress];
      
      if (inst == CodeInstruction::NOP){
	return;
      }

      if (inst == CodeInstruction::JUMP){
	
      }
    }

    void step()
    {

      if (getCurrentInstruction() == CodeInstruction::PROGRAM_START){
	_memory.allocateBlock(100);
      }
      
      if (getCurrentInstruction() == CodeInstruction::JUMP){
	_currentInstructionAddress ++;
	_currentInstructionAddress = static_cast<unsigned>(unsigned getCurrentInstruction());
	return;
      }

      if (getCurrentInstruction() == CodeInstruction::DECLARE){
	_currentInstructionAddress ++;
	int value_type = -1;
	int qualifier = -1;
	if (getCurrentInstruction() == CodeInstruction::TYPE_OF){
	  _currentInstructionAddress ++;
	  value_type = static_cast<int>(getCurrentInstruction());
	}
	_currentInstructionAddress ++;
	Value *valuePtr = nullptr;
	if (getCurrentInstruction() == CodeInstruction::LITERAL){
	  _currentInstructionAddress ++;
	  valuePtr = &_literals[static_cast<unsigned>(getCurrentInstruction())];
	}
	else if (getCurrentInstruction() == CodeInstruction::LITERAL){
	  valuePtr = &_memory.getLastBlockRef().getValue(static_cast<unsigned>(getCurrentInstruction()));
	}

	value = ;
	
	_memory.getLastBlockRef().allocate(*valuePtr);
      }

      if (getCurrentInstruction() == CodeInstruction::ASSIGN){
	
      }
	  
    }

    Value getRValue()
    {
      auto base_lambda = [](Value &value){return value};
      _currentInstructionAddress ++;
      while (getCurrentInstruction(-1) != CodeInstruction::LITERAL &&
	     getCurrentInstruction(-1) != CodeInstruction::STORED
	     ){
	
	if (getCurrentInstruction(-1) == CodeInstruction::ADD){
	  _stack.emplace_back();
	}
      }
      if (getCurrentInstruction(-1) == CodeInstruction::LITERAL){
	return base_lambda(getLiteral());
      }
      if (getCurrentInstruction(-1) == CodeInstruction::STORED){
	return base_lambda(getStored());
      }
    }

    
  private:
    std::vector<CodeInstruction> _instructions;
    std::vector<Value> _stack;
    unsigned _currentInstructionAddress{0};
    std::vector<Value> _literals;
    Memory _memory;

    CodeInstruction &getCurrentInstruction(int offset) const
    {
      return _instruction[_currentInstructionAddress + offset];
    }

    CodeInstruction &getCurrentInstruction() const
    {
      return getCurrentInstruction(0);
    }

    const Value getLiteral()
    {
      _currentInstructionAddress ++;
      return _literals[static_cast<unsigned>(getCurrentInstruction(-1))];
    }

    const Value getStored()
    {
      _currentInstructionAddress ++;
      return _memory.getLastBlockRef().getValue(static_cast<unsigned>(getCurrentInstruction(-1)));
    }
    
  }
}

#endif // A_CODE_EXECUTABLE_HPP


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
6   0,
7   TEST,
8   STORED,
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
