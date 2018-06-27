#ifndef A_TERMINAL_TOKEN_HPP
#define A_TERMINAL_TOKEN_HPP

#include "AbstractSymbol.hpp"
#include "TokenTypes.hpp"

namespace A
{
  class TerminalSymbol : public AbstractSymbol
  {
  public:
    TerminalSymbol(unsigned pos, std::string str);
    ~TerminalSymbol();
    
  private:
    TerminalName::T _symbolName;
  };
}

#ENDIF // A_TERMINAL_TOKEN_HPP
