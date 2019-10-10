#ifndef A_INSTRUCTION_POINTER
#define A_INSTRUCTION_POINTER

#include <vector>
#include "CodeInstructions.hpp"

namespace A
{
  class InstructionPointer
  {
  public:
    InstructionPointer(const std::vector<CodeInstruction> &instructions,
		unsigned initInstructionAddress)
      :_instructions(&instructions),
       _currentInstructionAddress(initInstructionAddress){}

    InstructionPointer(const std::vector<CodeInstruction> &instructions) : InstructionPointer(instructions, 0){}

    bool isAtEnd() const
    {
      return _currentInstructionAddress >= _instructions->size();
    }
    
    CodeInstruction getInstruction()
    {
      if (isAtEnd()){
	return CodeInstruction::NONE;
      }				      
      CodeInstruction c =  _instructions->at(_currentInstructionAddress);
      advance();
      return c;
    }
    
    CodeInstruction readInstruction(unsigned relativePosition) const
    {
      if (_currentInstructionAddress + relativePosition >= _instructions->size()){
	return CodeInstruction::NONE;
      }
      return _instructions->at(_currentInstructionAddress + relativePosition);
    }

    CodeInstruction readInstruction() const
    {
      if (isAtEnd()){
	return CodeInstruction::NONE;
      }
      return _instructions->at(_currentInstructionAddress);
    }

    void advance()
    {
      if (isAtEnd()){
	return;
      }
      _currentInstructionAddress ++;
    }

    void advanceOverNOPs()
    {
      do{
	advance();
      }while(readInstruction() == CodeInstruction::NOP)
    }

    void advance(unsigned instructionCount)
    {
      for (unsigned i = 0; i < instructionCount && !isAtEnd(); i++){
	_currentInstructionAddress ++;
      }
    }

    void advanceOverNOPs(unsigned instructionCount)
    {
      for (unsigned i = 0; i < instructionCount && !isAtEnd(); i++){
	do{
	  _currentInstructionAddress ++;
	}while(readInstruction() == CodeInstruction::NOP)
      }
    }

    unsigned getCurrentInstructionAddress()
    {
      return _currentInstructionAddress;
    } 
    
  private:
    const std::vector<CodeInstruction> *_instructions;
    unsigned _currentInstructionAddress;
  };
}

#endif // A_INSTRUCTION_POINTER
