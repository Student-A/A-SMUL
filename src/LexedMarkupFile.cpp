#include "LexedMarkupFile.h"

#include <fstream>

A::LexedMarkupFile::LexedMarkupFile(std::string fileDir)
{
  // ctor
  _tokenizer = A::Tonkenizer(fileDir);
  return;
}

A::LexedMarkupFile::~LexedMarkupFile()
{
  // dtor
  return;
}

unsigned A::LexedMarkupFile::getLineCount()
{
  return _tokenizer.getTokenLinesCount();
}

void A::LexedMarkupFile::seekLine(unsigned lineNumber)
{
  if (lineNumber >= getLineCount()) {
    // throw Exception
  }
  _currentLineNumber = lineNumber;
  if (_currentLineNumber == getLineCount() - 1) {
    _isEndOfFile = true;
  } else if (_isEndOfFile) {
    _isEndOfFile = false;
  }
}

A::MarkupLine A::LexedMarkupFile::getLine()
{
  if (_isEndOfFile){
    // throw Exception 
  } else if (_currentLineNumber == getLineCount() - 1) {
    _isEndOfFile = true;
    return _tokenizer.getLines()[_currentLineNumber];
  } else {
    return _tokenizer.getLines()[_currentLineNumber ++];
  }
}

unsigned A::LexedMarkupFile::getCurrentLineNumber()
{
  return _currentLineNumber;
}

bool A::LexedMarkupFile::isEnd()
{
  return _isEndOfFile;
}

void A::LexedMarkupFile::seekEnd()
{
  _currentLineNumber = getLineCount() - 1;
  _isEndOfFile = true;
}
