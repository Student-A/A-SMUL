#ifndef A_TOKENIZED_LINE_INFO_HPP
#define A_TOKENIZED_LINE_INFO_HPP

namespace A
{
  class TokenizedLineInfo
  {
  public:
    
    TokenizedLineInfo(unsigned beginTokenIndex, unsigned endTokenIndex)
      : _beginTokenIndex(beginTokenIndex), _endTokenIndex(endTokenIndex) {};
    
    ~TokenizedLineInfo() {};
    
  private:
    unsigned _beginTokenIndex, _endTokenIndex;
  };
}

#endif // A_TOKENIZED_LINE_INFO_HPP
