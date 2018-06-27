#ifndef A_CODE_EXECUTABLE_HPP
#define A_CODE_EXECUTABLE_HPP

#include "CodeInstructions.hpp"
#include "Memory.hpp"

namespace A
{
  class CodeExecutable
  {
  public:
    CodeExecutable(Memory *memory);

    ~CodeExecutable();
    
    void execute()
    {
      CodeInstruction inst = _instructions[_currentInstructionAddress];
      
    }
    
  private:
    std::vector<CodeInstruction> _instructions;
    unsigned _currentInstructionAddress{0};
  }
}

#endif // A_CODE_EXECUTABLE_HPP
