#ifndef A_MEMORY_BLOCK_HPP
#define A_MEMORY_BLOCK_HPP

#include "AValue.hpp"

namespace A
{
  class MemoryBlock
  {
  public:
    MemoryBlock(unsigned size)
    {
      _data.reserve(size);
    }

    MemoryBlock()
    {
    }
    
    ~MemoryBlock();

    unsigned allocate(AValue value)
    {
      if (_vacancies.size() > 0){
	std::get<0>(_data[_vacancies.back()]) = value;
	std::get<1>(_data[_vacancies.back()]) = true;
      }
      _data.push_back(std::make_tuple(value, true));
    }

    void deallocate(unsigned address)
    {
      if (address >= _data.size() || !std::get<1>(_data[address])){
	return;
      }
      std::get<1>(_data[address]) = false;
      _vacancies.push_back(address);
    }

  private:
    std::vector<std::tuple<Value, bool>> _data;
    std::vector<unsigned> _vacancies;
  }
}

#endif // A_MEMORY_BLOCK_HPP
