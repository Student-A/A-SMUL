#ifndef A_FILE_POINTER
#define A_FILE_POINTER

#include <vector>
#include <string>

namespace A
{
  class FilePointer
  {
  public:
    FilePointer(const std::string &fileContent,
		unsigned initPosition,
		unsigned initLine)
      :_fileContent(&fileContent),
       _currentLineNumber(initLine),
       _currentCharNumber(initPosition){}

    FilePointer(const std::string &fileContent) : FilePointer(fileContent, 0, 0){}

    bool isEOF() const
    {
      return _currentCharNumber >= _fileContent->size();
    }
    
    char getChar()
    {
      if (isEOF()){
	return '\0';
      }				      
      char c =  _fileContent->at(_currentCharNumber);
      advance();
      return c;
    }
    
    std::string getSubstring(unsigned len_)
    {
      if (isEOF()){
	return "";
      }
      len_ = _currentCharNumber + len_ < _fileContent->size() ? len_ : _fileContent->size() - _currentCharNumber;
      std::string sub = _fileContent->substr(_currentCharNumber, len_);
      advance(len_);
      return sub;
    }

    std::string readSubstring(unsigned len_)
    {
      if (isEOF()){
	return "";
      }
      len_ = _currentCharNumber + len_ < _fileContent->size() ? len_ : _fileContent->size() - _currentCharNumber;
      return _fileContent->substr(_currentCharNumber, len_);
    }
    
    char readChar(unsigned relativePosition) const
    {
      if (_currentCharNumber + relativePosition >= _fileContent->size()){
	return '\0';
      }
      return _fileContent->at(_currentCharNumber + relativePosition);
    }

    char readChar() const
    {
      if (isEOF()){
	return '\0';
      }
      return _fileContent->at(_currentCharNumber);
    }

    void advance()
    {
      if (isEOF()){
	return;
      }
      _currentCharNumber ++;
      if (_fileContent->at(_currentLineNumber) == '\n'){
	_currentLineNumber ++;
	_currentColumnNumber = 1;
      }
      _currentColumnNumber ++;
    }

    void advance(unsigned charCount)
    {
      for (unsigned i = 0; i < charCount && !isEOF(); i++){
	_currentCharNumber ++;
	if (_fileContent->at(_currentLineNumber) == '\n'){
	  _currentLineNumber ++;
	  _currentColumnNumber = 1;
	}
	_currentColumnNumber ++;
      }
    }
    
    unsigned getCurrentLineNumber()
    {
      return _currentLineNumber;
    }

    unsigned getCurrentColumnNumber()
    {
      return _currentColumnNumber;
    }    
    
  private:
    const std::string *_fileContent;
    unsigned _currentLineNumber;
    unsigned _currentColumnNumber;
    unsigned _currentCharNumber;
  };
}

#endif // A_FILE_POINTER
