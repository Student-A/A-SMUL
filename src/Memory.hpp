#ifndef A_MEMORY_HPP
#define A_MEMORY_HPP

#include "MemoryBlock.hpp"

namespace A
{
  struct MemoryAddress
  {
    unsigned block = -1;
    unsigned address = -1; 
  }
  
  class Memory
  {
  public:
    Memory()
    {
    }

    ~Memory()
    {
      for (auto &m : _memoryBlocks){
	delete m;
      }
      _memoryBlocks.empty();
    }

    unsigned allocateBlock(unsigned size)
    {
      _memoryBlocks.push_back(new MemoryBlock(size));
    }

    MemoryBlock &getLastBlockRef()
    {
      return *_memoryBlocks.back();
    }
    
  private:
    std::vector<MemoryBlock*> _memoryBlocks;
    
  }
}

#endif // A_MEMORY_HPP
