#ifndef A_MEMORY_BLOCK_HPP
#define A_MEMORY_BLOCK_HPP

#include "Value.hpp"

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

    unsigned allocate(Value value)
    {
      if (_vacancies.size() > 0){
	unsigned address = _vacancies.back();
	_vacancies.pop_back();
	std::get<0>(_data[address]) = value;
	std::get<1>(_data[address]) = true;
	return address;
      }
      _data.push_back(std::make_tuple(value, true));
      return (_data.size() - 1);
    }

    void deallocate(unsigned address)
    {
      if (address >= _data.size() || !std::get<1>(_data[address])){
	return;
      }
      std::get<1>(_data[address]) = false;
      _vacancies.push_back(address);
    }

    Value getValue(usngiend address)
    {
      return _data[address];
    }

  private:
    std::vector<std::tuple<Value, bool>> _data;
    std::vector<unsigned> _vacancies;
  }
}

#endif // A_MEMORY_BLOCK_HPP
