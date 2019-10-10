#ifndef A_CODE_EXECUTABLE_HPP
#define A_CODE_EXECUTABLE_HPP

#include "CodeInstructions.hpp"
#include "SymanticContext.hpp"
#include "Value.hpp"

namespace A
{
  class CodeExecutable
  {
  public:
    CodeExecutable(std::vector<CodeInstruction> instructions, std::vector<Value> literals) : _instructions(instructions), _literals(literals)
    {
    }

    ~CodeExecutable();
    
    void execute(SymanticContext &symContext)
    {
      CodeInstruction inst = _instructions[_currentInstructionAddress];
      if (inst == PROGRAM_START)
    }

    void executeCycle(SymanticContext &symContext)
    {
      CodeInstruction inst = _instructions[_currentInstructionAddress];
      
      if (inst == CodeInstruction::NOP){
	return;
      }

      if (inst == CodeInstruction::JUMP){
	
      }
    }

    void step(SymanticContext &symContext)
    {
      if (getCurrentInstruction() == CodeInstruction::PROGRAM_START){
        symContext.getMemoryRef().allocateBlock(100);
      }
      
      if (getCurrentInstruction() == CodeInstruction::JUMP){
	_currentInstructionAddress ++;
	_currentInstructionAddress = static_cast<unsigned>(getCurrentInstruction());
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
	  valuePtr = &symContext.getMemoryRef().getLastBlockRef().getValue(static_cast<unsigned>(getCurrentInstruction()));
	}

	value = ;
	
	symContext.getMemoryRef().getLastBlockRef().allocate(*valuePtr);
      }

      if (getCurrentInstruction() == CodeInstruction::ASSIGN){
	
      }
	  
    }

    Value getRValue(SymanticContext &symContext)
    {
      auto base_lambda = [](Value &value){return value};
      _currentInstructionAddress ++;
      while (symContext.getInstructionPtr().readInstruction() != CodeInstruction::LITERAL &&
	     symContext.getInstructionPtr().readInstruction() != CodeInstruction::STORED
	     ){
	
	if (symContext.getInstructionPtr().readInstruction() == CodeInstruction::ADD){
	  _stack.emplace_back();
	}
      }
      if (symContext.getInstructionPtr().readInstruction() == CodeInstruction::LITERAL){
	symContext.getInstructionPtr().advanceOverNOPs();
	return base_lambda(getLiteral());
      }
      if (symContext.getInstructionPtr().readInstruction() == CodeInstruction::STORED){
	symContext.getInstructionPtr().advanceOverNOPs();
	return base_lambda(getStored());
      }
    }

    
  private:
    std::vector<CodeInstruction> _instructions;
    std::vector<Value> _literals;

    const Value getLiteral(SymanticContext &symContext)
    {
      return _literals[static_cast<unsigned>(symContext.getInstructionPtr().getInstruction())];
    }

    const Value getStored(SymanticContext &symContext)
    {
      return symContext.getMemoryRef().getLastBlockRef().getValue(static_cast<unsigned>(symContext.getInstructionPtr().getInstruction()));
    }
    
  }
}

#endif // A_CODE_EXECUTABLE_HPP
