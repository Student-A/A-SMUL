
#ifndef A_SYMANTIC_CONTEXT
#define A_SYMANTIC_CONTEXT

#include "Memory.hpp"
#include "InstructionPointer.hpp"

namespace A
{
  class SymanticContext
  {
  public:
    SymanticContext(Memory *memory, const std::vector<CodeInstruction> &instructions): _instructionPointer(instructions)
    {
    }

    ~SymanticContext(){}

    Memory &getMemoryRef()
    {
      return *_memory;
    }

    InstructionPointer &getInstructionPtr()
    {
      return _instructionPointer;
    }
    
  private:
    Memory *_memory;
    std::vector<Value> _stack;
    InstructionPointer _instructionPointer;
  };
}

#endif // A_SYMANTIC_CONTEXT
