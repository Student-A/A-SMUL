#ifndef A_LEXED_MARKUP_FILE_HPP
#define A_LEXED_MARKUP_FILE_HPP

#include "LexedLineInfo.hpp"

#include <vector>
#include <string>

namespace A
{
  class LexedMarkupFile
  {
  public:
    LexedMarkupFile(std::string fileDir);

    ~LexedMarkupFile();

    unsigned getLineCount();

    void seekLine(unsigned i);

    unsigned getCurrentLineNumber();

    MarkupLine getLine();

    bool isEnd();

    void seekEnd();

  private: 
      
    // std::vector<LexedLineInfo> _tokenLinesInfo;
    bool _isEndOfFile{false};
    unsigned _currentLineNumber{0};
    Tokenizer _tokenizer;
  };
}

#endif // A_LEXED_MARKUP_FILE_HPP
