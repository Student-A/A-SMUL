#ifndef SYNTACTIC_CONTEXT_HPP
#define SYNTACTIC_CONTEXT_HPP

#include <algorithm>
#include <vector>
#include <tuple>
#include <string>

#include "TokenTypes.hpp"
#include "FilePointer.hpp"
#include "Token.hpp"

namespace A
{
  class SyntacticContext
  {
  public:
    SyntacticContext(FilePointer filePointer, std::vector<std::tuple<NestLevel, std::string>> nestLevels):_filePointer(filePointer){
      _nestLevels = nestLevels;
    }
    
    SyntacticContext(FilePointer filePointer):_filePointer(filePointer)
    {
      _nestLevels.push_back(std::make_tuple(NestLevel::UNNESTED, std::string()));
    }

    ~SyntacticContext(){}

    bool isLastLevel(NestLevel nestLevel)
    {
      return (_nestLevels.size() > 0 && (std::get<0>(_nestLevels.back()) & nestLevel) == std::get<0>(_nestLevels.back()));
    }

    bool isLastLevelAny(NestLevel nestLevel)
    {
      return (std::get<0>(_nestLevels.back()) & nestLevel) != NestLevel(0);
    }

    bool hasLevel(NestLevel nestLevel)
    {
      return (std::find_if(_nestLevels.begin(), _nestLevels.end(), [&](auto e){return (std::get<0>(e) & nestLevel) == std::get<0>(e);}) != _nestLevels.end());
    }

    const FilePointer getFilePointer()
    {
      return _filePointer;
    }

    FilePointer &getFilePointerRef()
    {
      return _filePointer;
    }

    void setFilePointer(FilePointer filePointer)
    {
      _filePointer = filePointer;
    }

    const std::vector<std::tuple<NestLevel, std::string>> &getNestLevels()
    {
      return _nestLevels;
    }

    void setNestLevels(std::vector<std::tuple<NestLevel, std::string>> &nestLevels)
    {
      _nestLevels = nestLevels;
    }

    Token getLastToken()
    {
      return _lastToken;
    }

    void setLastToken(const Token &lastToken)
    {
      _lastToken = lastToken;
    }

    bool isLastTokenType(MarkupTokenType tokenType)
    {
      return _lastToken.isTokenType(tokenType);
    }

    bool isLastTokenType(ScriptTokenType tokenType)
    {
      return _lastToken.isTokenType(tokenType);
    }

    bool isLastTokenType(RValueTokenType tokenType)
    {
      return _lastToken.isTokenType(tokenType);
    }

    bool hasLastTokenType(MarkupTokenType tokenType)
    {
      return _lastToken.hasTokenType(tokenType);
    }

    bool hasLastTokenType(ScriptTokenType tokenType)
    {
      return _lastToken.hasTokenType(tokenType);
    }

    bool hasLastTokenType(RValueTokenType tokenType)
    {
      return _lastToken.hasTokenType(tokenType);
    }

    bool isLastTokenClass(TokenClass tokenClass)
    {
      return _lastToken.isTokenClass(tokenClass);
    }

    void pushNestLevel(NestLevel nestLevel)
    {
      _nestLevels.push_back(std::make_tuple(nestLevel, std::string()));
    }

    void pushNestLevel(NestLevel nestLevel, std::string name)
    {
      _nestLevels.push_back(std::make_tuple(nestLevel, name));
    }

    NestLevel getLastNestLevel()
    {
      return std::get<0>(_nestLevels.back());
    }
    
    std::string &getLastNestLevelName()
    {
      return std::get<1>(_nestLevels.back());
    }

    void popLastNestLevel()
    {
      _nestLevels.pop_back();
    }

    void setLastComponentName(const std::string &str)
    {
      _lastComponentName = str;
    }
    
    std::string &getLastComponentName()
    {
      return _lastComponentName;
    }
    
  private:
    std::vector<std::tuple<NestLevel, std::string> > _nestLevels;
    FilePointer _filePointer;
    Token _lastToken;
    std::string _lastComponentName{""};
  };
}

#endif // SYNTACTIC_CONTEXT_HPP
