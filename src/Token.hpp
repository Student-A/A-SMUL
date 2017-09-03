#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <cstdint>

#include "TokenTypes.hpp"
#include "FilePointer.hpp"

#include <iostream>

namespace A
{
  class Token
  {
  public:
    Token():_tokenPosition(_tokenString, 0, 0){};
    Token(FilePointer pos, std::uint64_t tokenType): _tokenType(tokenType), _tokenPosition(pos){};
    Token(FilePointer pos, std::uint64_t tokenType, const std::string &tokenString):_tokenType(tokenType), _tokenString(tokenString), _tokenPosition(pos){};
    ~Token(){};

    std::uint64_t getTokenType()
    {
      return _tokenType;
    }
    
    const std::string &getTokenString()
    {
      return _tokenString;
    }

    void setTokenString(const std::string &tokenString)
    {
      _tokenString = tokenString;
    }

    TokenClass getTokenClass()
    {
      return _tokenClass;
    }

    FilePointer getTokenPosition()
    {
      return _tokenPosition;
    }
      
    void setTokenClass(TokenClass tokenClass)
    {
      _tokenClass = tokenClass;
    }

    bool isTokenClass(TokenClass tokenClass)
    {
      return _tokenClass == tokenClass;
    }
    
    bool isTokenType(std::uint64_t tokenType)
    {
      return ((_tokenType & tokenType) == _tokenType);
    }

    bool isTokenType(MarkupTokenType tokenType)
    {
      if (_tokenClass != TokenClass::MARKUP_TOKEN)
	return false;
      return isTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool isTokenType(ScriptTokenType tokenType)
    {
      if (_tokenClass != TokenClass::SCRIPT_TOKEN)
	return false;
      return isTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool isTokenType(RValueTokenType tokenType)
    {
      if (_tokenClass != TokenClass::RVALUE_TOKEN)
	return false;
      return isTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenType(std::uint64_t tokenType)
    {
      return ((_tokenType & tokenType) == tokenType);
    }

    bool hasTokenType(MarkupTokenType tokenType)
    {
      if (_tokenClass != TokenClass::MARKUP_TOKEN)
	return false;
      return hasTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenType(ScriptTokenType tokenType)
    {
      if (_tokenClass != TokenClass::SCRIPT_TOKEN)
	return false;
      return hasTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenType(RValueTokenType tokenType)
    {
      if (_tokenClass != TokenClass::RVALUE_TOKEN)
	return false;
      return hasTokenType(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenTypeAny(unsigned tokenType)
    {
      return ((_tokenType & tokenType) != 0);
    }

    bool hasTokenTypeAny(MarkupTokenType tokenType)
    {
      if (_tokenClass != TokenClass::MARKUP_TOKEN)
	return false;
      return hasTokenTypeAny(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenTypeAny(ScriptTokenType tokenType)
    {
      if (_tokenClass != TokenClass::SCRIPT_TOKEN)
	return false;
      return hasTokenTypeAny(static_cast<std::uint64_t>(tokenType));
    }

    bool hasTokenTypeAny(RValueTokenType tokenType)
    {
      if (_tokenClass != TokenClass::RVALUE_TOKEN)
	return false;
      return hasTokenTypeAny(static_cast<std::uint64_t>(tokenType));
    }

    std::string toString()
    {
      switch (_tokenClass){
      case TokenClass::MARKUP_TOKEN:
	if (!_tokenString.empty()){
	  return (MARKUP_TOKEN_TYPES_STRINGS.at(static_cast<MarkupTokenType>(_tokenType)) + "(" + _tokenString + ")");
	}
	return (MARKUP_TOKEN_TYPES_STRINGS.at(static_cast<MarkupTokenType>(_tokenType)));
	
      case TokenClass::SCRIPT_TOKEN:
	if (!_tokenString.empty()){
	  return (SCRIPT_TOKEN_TYPES_STRINGS.at(static_cast<ScriptTokenType>(_tokenType)) + "(" + _tokenString + ")");
	}
	return (SCRIPT_TOKEN_TYPES_STRINGS.at(static_cast<ScriptTokenType>(_tokenType)));

      case TokenClass::RVALUE_TOKEN:
	if (!_tokenString.empty()){
	  return (RVALUE_TOKEN_TYPES_STRINGS.at(static_cast<RValueTokenType>(_tokenType)) + "(" + _tokenString + ")");
	}
	return (RVALUE_TOKEN_TYPES_STRINGS.at(static_cast<RValueTokenType>(_tokenType)));

      default:
	return "Unknown";
      }
    }
    
  protected:
    TokenClass _tokenClass{TokenClass::NO_CLASS};
    std::uint64_t _tokenType{0};
    std::string _tokenString;
    FilePointer _tokenPosition;
  };

  class MarkupToken : public Token
  {
  public:
    MarkupToken(FilePointer pos, MarkupTokenType tokenType) : Token(pos, static_cast<std::uint64_t>(tokenType))
    {
      _tokenClass = TokenClass::MARKUP_TOKEN;
    }
    
    MarkupToken(FilePointer pos, MarkupTokenType tokenType, const std::string &tokenString) : Token(pos, static_cast<std::uint64_t>(tokenType), tokenString)
    {
      _tokenClass = TokenClass::MARKUP_TOKEN;
    }

    MarkupTokenType getTokenType()
    {
      return static_cast<MarkupTokenType>(_tokenType);
    }
  };

  class RValueToken : public Token
  {
  public:
    RValueToken(FilePointer pos, RValueTokenType tokenType) : Token(pos, static_cast<std::uint64_t>(tokenType))
    {
      _tokenClass = TokenClass::RVALUE_TOKEN;
    }
    
    RValueToken(FilePointer pos, RValueTokenType tokenType, const std::string &tokenString) : Token(pos, static_cast<std::uint64_t>(tokenType), tokenString)
    {
      _tokenClass = TokenClass::RVALUE_TOKEN;
    }
    
    RValueTokenType getTokenType()
    {
      return static_cast<RValueTokenType>(_tokenType);
    }
  };

  class ScriptToken : public Token
  {
  public:
    ScriptToken(FilePointer pos, ScriptTokenType tokenType) : Token(pos, static_cast<std::uint64_t>(tokenType))
    {
      _tokenClass = TokenClass::SCRIPT_TOKEN;
    }
    
    ScriptToken(FilePointer pos, ScriptTokenType tokenType, const std::string &tokenString) : Token(pos, static_cast<std::uint64_t>(tokenType), tokenString)
    {
      _tokenClass = TokenClass::SCRIPT_TOKEN;
    }

    ScriptTokenType getTokenType()
    {
      return static_cast<ScriptTokenType>(_tokenType);
    }
  };
}

#endif // TOKEN_HPP
