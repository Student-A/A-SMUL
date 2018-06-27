#ifndef A_MARKUP_LINE_HPP
#define A_MARKUP_LINE_HPP

namespace A
{
  class MarkupLine
  {
  public:
    MarkupLine(std::string line);

    MarkupLine(char c);

    ~MarkupLine();

    void seek(unsigned tokenId);
    
    Token getNextToken();
    
  private:
    unsigned _lineNumber;
    
    Token** _lineTokens;
  };
}

#endif // A_MARKUP_LINE_HPP
