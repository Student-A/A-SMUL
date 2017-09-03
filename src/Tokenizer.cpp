#include "Tokenizer.hpp"
#include "TokenTypes.hpp"

#include "GeneralException.hpp"
#include "TokenizerErrorStrings.hpp"

#include <cstring>

#include <iostream>

namespace A
{
  Tokenizer::Tokenizer(const std::string *source):_source(source), _context(FilePointer(*source))
  {
  }

  Tokenizer::Tokenizer(const std::string *source, SyntacticContext ctx):_source(source), _context(ctx)
  {
  }

  Tokenizer::~Tokenizer()
  {
  }
  
  Token Tokenizer::getNextToken()
  {
    return getNextToken(_context);
  }

  Token Tokenizer::getNextToken(SyntacticContext &ctx)
  {
    if (ctx.hasLevel(NestLevel::RVALUE)){
      return getNextRValueToken(ctx);
    }
    else if (ctx.hasLevel(NestLevel::MARKUP)){
      return getNextMarkupToken(ctx);
    }else {
      return getNextScriptToken(ctx);
    }
  }

  Token Tokenizer::getMarkupCharacterDependentToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    char c = ctx.getFilePointerRef().readChar();
    if (c == '}'){
      if (ctx.isLastLevel(NestLevel::MARKUP_MAIN_COMPONENT)){
	// then it must be after either an main component slash token or an main component closing name
	if (ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_CLOSING_SLASH) || ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_CLOSING_NAME)){
	  return getMainComponentClosingNameClosingToken(ctx);
	}
      }	    
    }
    else if (c == ']'){
      if (ctx.isLastLevel(NestLevel::MARKUP_SUB_COMPONENT)){
	return getSubComponentClosingToken(ctx);
      }else if (ctx.isLastLevel(NestLevel::MARKUP_ID_COMPONENT)){
	// then it must be after either an id component slash token or an id component closing id name
	if (ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_CLOSING_SLASH) || ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_CLOSING_ID_NAME)){
	  return getIdComponentClosingIdNameClosingToken(ctx);
	}
      }	    
    }
    else if (isalpha(c) && ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_CLOSING_SLASH)){
      return getMainComponentClosingNameToken(ctx);
    }
    else if (isalpha(c) && ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_CLOSING_SLASH)){
      return getIdComponentClosingIdNameToken(ctx);
    }
    else if (c == '['){
      if (ctx.isLastLevel(NestLevel::MARKUP_ID_COMPONENT)){
	// look ahead to see if new id component or end of current
	FilePointer tmp = ctx.getFilePointer();
	c = getNextNonSpaceChar(tmp);
	c = getNextNonSpaceChar(tmp);
        if (c == '/'){
	  return getIdComponentClosingToken(ctx);
	}
      }
      return getIdComponentOpeningToken(ctx);
    }
    else if (c == '{'){
      if (ctx.isLastLevel(NestLevel::MARKUP_MAIN_COMPONENT)){
	// look ahead to see if new main component or end of current
	FilePointer tmp = ctx.getFilePointer();
	c = getNextNonSpaceChar(tmp);
	c = getNextNonSpaceChar(tmp);
        if (c == '/'){
	  return getMainComponentClosingToken(ctx);
	}
      }
      return getMainComponentOpeningToken(ctx);
    }
    else if (c == '#'){
      return getCommentToken(ctx);
    }
    else if (c == '/' &&  ctx.getFilePointer().readChar(1) == '#'){
      return getMultilineCommentOpeningToken(ctx);
    }
    else if (!(isalpha(c) || c == '_')){
      return getUndefinedToken(ctx);
    }

    FilePointer namePosition = ctx.getFilePointer();
    std::string name = getName(ctx.getFilePointerRef());

    char charAfterName = ctx.getFilePointer().readChar();
	
    getPastWhiteSpace(ctx.getFilePointerRef());

    c = ctx.getFilePointer().readChar();
    if (c == '='){
      MarkupToken tok(namePosition, MarkupTokenType::TAG_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == ':'){
      MarkupToken tok(namePosition, MarkupTokenType::SWITCH_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '['){
      ctx.setLastComponentName(name);
      MarkupToken tok(namePosition, MarkupTokenType::SUB_COMPONENT_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (charAfterName == ' ' || charAfterName == '\t' || charAfterName == '\n'){
      MarkupToken tok(namePosition, MarkupTokenType::STANDALONE_SWITCH, name);
      ctx.setLastToken(tok);
      return tok;
    }else{
      return getUndefinedToken(ctx);
    }
  }
  
  Token Tokenizer::getNextMarkupToken(SyntacticContext &ctx)
  {
    if (ctx.isLastLevel(NestLevel::MARKUP) || ctx.isLastLevelAny(NestLevel::COMPONENT)){
      if (ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_OPENING)){
	return getMainComponentOpeningNameToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_OPENING_NAME)){
	return getMainComponentOpeningNameClosingToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::SUB_COMPONENT_NAME)){
	return getSubComponentBracketToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::TAG_NAME)){
	return getTagEqualToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::MAIN_COMPONENT_CLOSING)){
	return getMainComponentClosingSlashToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_CLOSING)){
	return getIdComponentClosingSlashToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::SWITCH_NAME)){
	return getSwitchColonToken(ctx);
      }else{
        return getMarkupCharacterDependentToken(ctx);
      }
    }
    else if (ctx.isLastLevel(NestLevel::MARKUP_ID_COMPONENT_KEY)){
      if (ctx.isLastTokenClass(TokenClass::RVALUE_TOKEN)){
	getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
	if (ctx.getFilePointerRef().readChar() == ']'){
	  return getIdComponentIdClosingToken(ctx);
	}
	FilePointer namePosition = ctx.getFilePointer();
	std::string name = getName(ctx.getFilePointerRef());

	getPastWhiteSpace(ctx.getFilePointerRef());

	char c = ctx.getFilePointer().readChar();
	if (c == '='){
	  MarkupToken tok(namePosition, MarkupTokenType::TAG_NAME, name);
	  ctx.setLastToken(tok);
	  return tok;
	}else{
	  return getUndefinedToken(ctx);
	}
      }
      else if (ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_OPENING)){
	return getIdComponentIdNameToken(ctx);
      }
      else if (ctx.isLastTokenType(MarkupTokenType::ID_COMPONENT_ID_NAME)){
	return getIdComponentIdEqualToken(ctx);
      }else{
        throw GeneralException(A_TOKENIZER_ERROR_1);
      }
    }
    else{
      throw GeneralException(A_TOKENIZER_ERROR_0);
    }
  }
  
  Token Tokenizer::getMainComponentOpeningNameToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    std::string name = getName(ctx.getFilePointerRef());
    if (!name.empty()){
      ctx.setLastComponentName(name);
      MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_OPENING_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_2);
    }
  }

  Token Tokenizer::getMainComponentOpeningNameClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '}'){
      MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_OPENING_NAME_CLOSING);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::MARKUP_MAIN_COMPONENT, ctx.getLastComponentName());
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_3 + ", not '" +std::string(1, c) + "'");
    }
  }
  
  Token Tokenizer::getIdComponentIdNameToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    ctx.setLastComponentName(getName(ctx.getFilePointerRef()));

    MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_ID_NAME, ctx.getLastComponentName());
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getIdComponentIdEqualToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '='){
      MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_ID_EQUAL);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::RVALUE);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_4 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getIdComponentIdClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_ID_CLOSING);
    ctx.popLastNestLevel();
    ctx.pushNestLevel(NestLevel::MARKUP_ID_COMPONENT, ctx.getLastComponentName());
    ctx.setLastToken(tok);
    return tok;
  }
  
  Token Tokenizer::getSubComponentBracketToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '['){
      MarkupToken tok(tokenPos, MarkupTokenType::SUB_COMPONENT_BRACKET);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::MARKUP_SUB_COMPONENT, ctx.getLastComponentName());
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_5 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getSubComponentClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance(); 
    MarkupToken tok(tokenPos, MarkupTokenType::SUB_COMPONENT_CLOSING);
    ctx.setLastToken(tok);
    ctx.popLastNestLevel();
    return tok;
  }

  Token Tokenizer::getIdComponentClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_CLOSING);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getIdComponentClosingSlashToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '/'){
      MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_CLOSING_SLASH);
      ctx.setLastToken(tok);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_6 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getIdComponentClosingIdNameToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    std::string name = getName(ctx.getFilePointerRef());

    if (name == ctx.getLastNestLevelName()){ 
      MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_CLOSING_ID_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    }else{
      throw GeneralException(A_TOKENIZER_ERROR_7);
    }
  }

  Token Tokenizer::getIdComponentClosingIdNameClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == ']'){
      MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_CLOSING_ID_NAME_CLOSING);
      ctx.setLastToken(tok);
      ctx.popLastNestLevel();
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_8 + ", not '" + std::string(1, c) + "'");
    }
  }


  Token Tokenizer::getMainComponentClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();    
    MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_CLOSING);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getMainComponentClosingSlashToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '/'){
      MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_CLOSING_SLASH);
      ctx.setLastToken(tok);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_9 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getMainComponentClosingNameToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    std::string name = getName(ctx.getFilePointerRef());

    if (name == ctx.getLastNestLevelName()){ 
      MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_CLOSING_NAME, name);
      ctx.setLastToken(tok);
      return tok;
    }else{
      throw GeneralException(A_TOKENIZER_ERROR_10);
    }
  }

  Token Tokenizer::getMainComponentClosingNameClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_CLOSING_NAME_CLOSING);
    ctx.setLastToken(tok);
    ctx.popLastNestLevel();
    return tok;    
  }

  
  Token Tokenizer::getTagEqualToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == '='){
      MarkupToken tok(tokenPos, MarkupTokenType::TAG_EQUAL);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::RVALUE);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_11 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getSwitchColonToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().getChar();
    if (c == ':'){
      MarkupToken tok(tokenPos, MarkupTokenType::SWITCH_COLON);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::RVALUE);
      return tok;
    } else {
      throw GeneralException(A_TOKENIZER_ERROR_12 + ", not '" + std::string(1, c) + "'");
    }
  }

  Token Tokenizer::getIdComponentOpeningToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    MarkupToken tok(tokenPos, MarkupTokenType::ID_COMPONENT_OPENING);
    ctx.setLastToken(tok);
    ctx.pushNestLevel(NestLevel::MARKUP_ID_COMPONENT_KEY);
    return tok;    
  }

  Token Tokenizer::getMainComponentOpeningToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    MarkupToken tok(tokenPos, MarkupTokenType::MAIN_COMPONENT_OPENING);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getCommentToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    MarkupToken tok(tokenPos, MarkupTokenType::COMMENT_BEGIN);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getMultilineCommentOpeningToken(SyntacticContext &ctx)
  {
    // todo
    return Token();
  }

  Token Tokenizer::getContainerIndexToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();    
    RValueToken tok(tokenPos, RValueTokenType::CONTAINER_INDEX_OPENING);
    ctx.setLastToken(tok);
    ctx.pushNestLevel(NestLevel::RVALUE_CONTAINER_INDEX);
    return tok;
  }

  Token Tokenizer::getFunctionCallOpeningToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    RValueToken tok(tokenPos, RValueTokenType::FUNCTION_CALL_PARAMETER_LIST_OPENING);
    ctx.setLastToken(tok);
    ctx.pushNestLevel(NestLevel::RVALUE_FUNCTION_CALL_PARAMETER_LIST);
    return tok;
  }

  Token Tokenizer::getMemberOperatorToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer tokenPos = ctx.getFilePointer();
    ctx.getFilePointerRef().advance();
    RValueToken tok(tokenPos, RValueTokenType::MEMBER_OPERATOR);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getInfixOperatorToken(SyntacticContext &ctx)
  {
    // todo: allow custom infix operators of the alphabet  

    getPastWhiteSpace(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();

    char c = ctx.getFilePointerRef().readChar();
    
    // obvious to parse oeprators
    if (c == '+' || c == '/' || c == '^' || c == '%'){
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::INFIX_OPERATOR, std::string(1, c));
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '>' || c == '<' || c == '=' || c == '*' ||
	     c == '!' || c == '|' || c == '&' || c == '-'){
      const char c2 = ctx.getFilePointerRef().readChar(1);
      const char op[] = {c, c2, '\0'};
      // if 2-char operator is one of these
      if (strncmp(op, ">>", 2) == 0 || strncmp(op, ">=", 2) == 0 ||
	  strncmp(op, "<<", 2) == 0 || strncmp(op, "<=", 2) == 0 ||
	  strncmp(op, "==", 2) == 0 || strncmp(op, "!=", 2) == 0 ||
	  strncmp(op, "||", 2) == 0 || strncmp(op, "&&", 2) == 0 ||
	  strncmp(op, "**", 2) == 0 || strncmp(op, "->", 2) == 0){
	ctx.getFilePointerRef().advance(2);
	RValueToken tok(tokenPos, RValueTokenType::INFIX_OPERATOR, std::string(op));
	ctx.setLastToken(tok);
	return tok;
      }
    }
    else if (isalpha(c)) {
      FilePointer tmp = ctx.getFilePointer();
      std::string opStr = getWord(tmp);

      RValueToken tok(tokenPos, RValueTokenType::INFIX_OPERATOR, opStr);
      if (opStr == "and" || opStr == "or" || opStr == "if" || opStr == "else"){
	if (opStr == "if"){
	  ctx.pushNestLevel(NestLevel::RVALUE_CONDITION);
	}
	else if (ctx.isLastLevel(NestLevel::RVALUE_CONDITION) && opStr == "else"){
	  ctx.popLastNestLevel();
	}
      }else{
	throw GeneralException(A_TOKENIZER_ERROR_15 + ": '" + opStr + "'");
      }

      ctx.getFilePointerRef().advance(opStr.size());
      ctx.setLastToken(tok);
      return tok;
    }
    throw GeneralException(A_TOKENIZER_ERROR_15 + ": '" + std::string(1, c) + "'");
  }

  bool Tokenizer::doesOperatorFollow(A::SyntacticContext &ctx)
  {
    getPastWhiteSpace(ctx.getFilePointerRef());

    const char c = ctx.getFilePointerRef().readChar();
    const char c2 = ctx.getFilePointerRef().readChar(1);
    const char op[] = {c, c2, '\0'};
    
    // obvious to parse oeprators
    if (c == '+' || c == '/' || c == '^' || c == '%' ||
	strncmp(op, ">>", 2) == 0 || strncmp(op, ">=", 2) == 0 ||
	strncmp(op, "<<", 2) == 0 || strncmp(op, "<=", 2) == 0 ||
	strncmp(op, "==", 2) == 0 || strncmp(op, "!=", 2) == 0 ||
	strncmp(op, "||", 2) == 0 || strncmp(op, "&&", 2) == 0 ||
	strncmp(op, "**", 2) == 0 || strncmp(op, "->", 2) == 0){
      return true;
    }else if (isalpha(c)) {
      FilePointer tmp = ctx.getFilePointer();
      std::string opStr = getWord(tmp);

      if (opStr == "and" || opStr == "or" || opStr == "if"){
	return true;
      }
      else if (opStr == "else" && ctx.isLastLevel(NestLevel::RVALUE_CONDITION) && opStr == "else"){
	return true;
      }else{
	return false;
      }
    }else{
      return false;
    }
  }

  Token Tokenizer::getTextClosingToken(A::SyntacticContext &ctx)
  {
    FilePointer tokenPos = ctx.getFilePointer();
    
    char c = ctx.getFilePointerRef().readChar();
    ctx.getFilePointerRef().advance();
    bool isSingleQuoteToken = (c == '\'' && ctx.isLastLevel(NestLevel::RVALUE_TEXT_SINGLE_QUOTED));
    bool isDoubleQuoteToken = (c == '"' && ctx.isLastLevel(NestLevel::RVALUE_TEXT_DOUBLE_QUOTED)); 
    if (isSingleQuoteToken || isDoubleQuoteToken){
      RValueToken tok(tokenPos, RValueTokenType::TEXT_CLOSING, std::string(1, c));
      ctx.setLastToken(tok);
      ctx.popLastNestLevel();
      return tok;
    }else{
      return getTextContentToken(ctx);
    }
  }
  
  Token Tokenizer::getTextContentToken(A::SyntacticContext &ctx)
  {
    FilePointer tokenPos = ctx.getFilePointer();
    char c = ctx.getFilePointerRef().readChar();

    bool escapeNextChar = false;
    unsigned textSize = 0;
    while (true){
      if (escapeNextChar){
	escapeNextChar = false;
	textSize ++;
	continue;
      }
      
      c = ctx.getFilePointerRef().readChar(textSize);
      if (c == '\\'){
	escapeNextChar = true;
      }

      bool isTextClosingChar = ((c == '"' && ctx.isLastLevel(NestLevel::RVALUE_TEXT_DOUBLE_QUOTED)) ||
				(c == '\'' && ctx.isLastLevel(NestLevel::RVALUE_TEXT_SINGLE_QUOTED)));
	
      if (isTextClosingChar || c == '\n'){ // ignoring eof for now
	std::string textContent = ctx.getFilePointerRef().getSubstring(textSize);
        RValueToken tok(tokenPos, RValueTokenType::TEXT_CONTENT, textContent);
	ctx.setLastToken(tok);
	return tok;
      }else{
	textSize ++;
      }
    }
  }
  
  Token Tokenizer::getValueToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();
    
    char c = ctx.getFilePointerRef().readChar();
    if (c == '-' || c == '+' || c == '~'){
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::PREFIX_OPERATOR, std::string(1, c));
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '"' || c == '\''){
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::TEXT_OPENING, std::string(1, c));
      ctx.setLastToken(tok);
      if (c == '"'){
	ctx.pushNestLevel(NestLevel::RVALUE_TEXT_DOUBLE_QUOTED);
      } else if (c == '\''){
	ctx.pushNestLevel(NestLevel::RVALUE_TEXT_SINGLE_QUOTED);
      }
      return tok;
    }
    else if (c == '.' || isdigit(c)) {
      bool hasDecimalPoint = false;
      unsigned numberLength = 0;
      while ((c == '.' && !hasDecimalPoint) || isdigit(c)){
	numberLength ++;
	if (c == '.'){
	  hasDecimalPoint = true;
	}
	c = ctx.getFilePointerRef().readChar(numberLength);
      }
      std::string numberStr = ctx.getFilePointerRef().getSubstring(numberLength);
      RValueToken tok(tokenPos, RValueTokenType::NUMBER, numberStr);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '_' || isalpha(c)) {
      std::string identifierName = getName(ctx.getFilePointerRef());
      RValueToken tok(tokenPos, RValueTokenType::IDENTIFIER, identifierName);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '<') {
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::LAMBDA_OPENING);
      ctx.pushNestLevel(NestLevel::RVALUE_LAMBDA_DEFINITION_PRE_PARAMETER_LIST);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '[') {
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::LIST_OPENING);
      ctx.pushNestLevel(NestLevel::RVALUE_LIST);
      ctx.setLastToken(tok);
      return tok;
    }
    else if (c == '(') {
      ctx.getFilePointerRef().advance();
      RValueToken tok(tokenPos, RValueTokenType::SUB_EXPRESSION_OPENING);
      ctx.pushNestLevel(NestLevel::RVALUE_SUB_EXPRESSION);
      ctx.setLastToken(tok);
      return tok;
    }
    else {
      return getUndefinedToken(ctx);
    }
  }

  Token Tokenizer::getRValueToken(A::SyntacticContext &ctx)
  {
    ctx.pushNestLevel(NestLevel::RVALUE);
    return getNextRValueToken(ctx);
  }
  
  Token Tokenizer::getUndefinedToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tokenPos = ctx.getFilePointer();

    std::string tokenStr = getWord(ctx.getFilePointerRef());
    
    Token tok(tokenPos, 0, tokenStr);
    ctx.setLastToken(tok);
    return tok;
  }

  Token Tokenizer::getConditionBodyClosingToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());
    
    FilePointer pos = ctx.getFilePointer();
    
    if (ctx.isLastLevel(NestLevel::SCRIPT_IF_BODY)){
      ScriptToken tok(pos, ScriptTokenType::IF_CONDITION_BODY_CLOSING);
      ctx.popLastNestLevel();
      ctx.getFilePointerRef().advance(3);
      return tok;
    }else if (ctx.isLastLevel(NestLevel::SCRIPT_WHILE_BODY)){
      ScriptToken tok(pos, ScriptTokenType::WHILE_CONDITION_BODY_CLOSING);
      ctx.popLastNestLevel();
      ctx.getFilePointerRef().advance(3);
      return tok;
    }else if (ctx.isLastLevel(NestLevel::SCRIPT_ELSEIF_BODY)){
      ScriptToken tok(pos, ScriptTokenType::ELSEIF_CONDITION_BODY_CLOSING);
      ctx.popLastNestLevel();
      ctx.getFilePointerRef().advance(3);
      return tok;
    }else if (ctx.isLastLevel(NestLevel::SCRIPT_ELSE_BODY)){
      ScriptToken tok(pos, ScriptTokenType::ELSE_CONDITION_BODY_CLOSING);
      ctx.popLastNestLevel();
      ctx.getFilePointerRef().advance(3);
      return tok;
    }else{
      throw GeneralException(A_TOKENIZER_ERROR_16);
    }
  }

  
  Token Tokenizer::getScriptCommandToken(SyntacticContext &ctx)
  {
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tmp = ctx.getFilePointer();

    FilePointer pos = ctx.getFilePointer();
    
    std::string word = getName(tmp);

    return getScriptCommandToken(ctx, word, pos);
  }
    
  Token Tokenizer::getScriptCommandToken(SyntacticContext &ctx, std::string &lookaheadWord, FilePointer &pos)
  {
    if (lookaheadWord == "let"){
      ScriptToken tok(pos, ScriptTokenType::DECLARATION_LET);
      ctx.setLastToken(tok);
      ctx.pushNestLevel(NestLevel::SCRIPT_NAME_DECLARATION);
      ctx.getFilePointerRef().advance(lookaheadWord.size());
      return tok;
    }
    else if (lookaheadWord == "if"){
      ScriptToken tok(pos, ScriptTokenType::IF_CONDITION_OPENING);
      ctx.pushNestLevel(NestLevel::SCRIPT_IF_CONDITION);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance(lookaheadWord.size());
      return tok;
    }
    else if (lookaheadWord == "while"){
      ScriptToken tok(pos, ScriptTokenType::WHILE_CONDITION_OPENING);
      ctx.pushNestLevel(NestLevel::SCRIPT_WHILE_CONDITION);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance(lookaheadWord.size());
      return tok;
    }
    else if (lookaheadWord == "function"){
      ScriptToken tok(pos, ScriptTokenType::FUNCTION_DEFINITION_OPENING);
      ctx.pushNestLevel(NestLevel::SCRIPT_FUNCTION_DECLARATION);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance(lookaheadWord.size());
      return tok;
    }
    else if (ctx.isLastLevelAny(NestLevel::SCRIPT_CONDITION_BODY) && lookaheadWord == "end"){
      return getConditionBodyClosingToken(ctx);
    }
    else if ((ctx.isLastLevel(NestLevel::SCRIPT_IF_BODY) || ctx.isLastLevel(NestLevel::SCRIPT_ELSEIF_BODY)) && (lookaheadWord == "elseif" || lookaheadWord == "else")){
      if (lookaheadWord == "elseif"){
	ScriptToken tok(pos, ScriptTokenType::ELSEIF_CONDITION_OPENING);
	ctx.pushNestLevel(NestLevel::SCRIPT_ELSEIF_CONDITION);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(lookaheadWord.size());
	return tok;
      } else if (lookaheadWord == "else"){
	ScriptToken tok(pos, ScriptTokenType::ELSE_CONDITION_BODY_OPENING);
	ctx.pushNestLevel(NestLevel::SCRIPT_ELSE_BODY);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(lookaheadWord.size());
	return tok;
      }
    }
    else if (ctx.getFilePointerRef().readChar() == '{'){
      ctx.pushNestLevel(NestLevel::MARKUP);
      return getNextMarkupToken(ctx); 
    }
    else if (!lookaheadWord.empty()){
      ScriptToken tok(pos, ScriptTokenType::LVALUE_NAME);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance(lookaheadWord.size());
      return tok;
    }
    return getRValueToken(ctx);
  }

  Token Tokenizer::getNextScriptToken(SyntacticContext &ctx)
  {
    if (!ctx.isLastTokenClass(TokenClass::SCRIPT_TOKEN) && ctx.isLastLevelAny(NestLevel::SCRIPT_COMMAND)){
      return getScriptCommandToken(ctx);
    }
    
    if (ctx.isLastTokenType(ScriptTokenType::DEFINITION_EQUAL)){
      return getRValueToken(ctx);
    }
    
    getPastWhiteSpaceAndLineBreaks(ctx.getFilePointerRef());

    FilePointer tmp = ctx.getFilePointer();

    FilePointer pos = ctx.getFilePointer();
    
    std::string word = getName(tmp);

    char c = ctx.getFilePointerRef().readChar();
      
    if (ctx.isLastTokenType(ScriptTokenType::TYPE_NAME) && c == '|'){
      ScriptToken tok(pos, ScriptTokenType::TYPE_DELIMETER);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance();
      return tok;
    }

    if (ctx.isLastTokenType(ScriptTokenType::TYPE_NAME) && ctx.getLastToken().getTokenString() == "list" && c == '['){
      ScriptToken tok(pos, ScriptTokenType::LIST_ELEMENTS_TYPE_OPENING);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance();
      return tok;
    }

    if (c == '=' && ctx.isLastTokenType(ScriptTokenType::LVALUE_NAME)){
      ScriptToken tok(pos, ScriptTokenType::DEFINITION_EQUAL);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance();
      return tok;
    }

    if ((ctx.isLastTokenType(ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_CLOSING) || ctx.isLastTokenType(ScriptTokenType::FUNCTION_VALUE_TYPE)) && c == ':'){
      ScriptToken tok(pos, ScriptTokenType::FUNCTION_DEFINITION_COLON);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance();
      ctx.pushNestLevel(NestLevel::SCRIPT_FUNCTION_BODY);
      return tok;
    }
      
    if (!word.empty() && ctx.isLastTokenType(ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_CLOSING)){
      ScriptToken tok(pos, ScriptTokenType::TYPE_NAME, word);
      ctx.setLastToken(tok);
      ctx.getFilePointerRef().advance(word.size());
      return tok;
    }
      
    if (ctx.isLastLevelAny(NestLevel::SCRIPT_COMMAND)){	
      getScriptCommandToken(ctx, word, pos);
    }
    else if (ctx.isLastLevel(NestLevel::SCRIPT_FUNCTION_DECLARATION)){
      if (ctx.isLastTokenType(ScriptTokenType::FUNCTION_NAME) && c == '('){
	ScriptToken tok(pos, ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_OPENING);
	ctx.pushNestLevel(NestLevel::SCRIPT_FUNCTION_PARAMETER_LIST);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance();
	return tok;
      }
      else if (ctx.isLastTokenType(ScriptTokenType::FUNCTION_DEFINITION_OPENING) && !word.empty()){
	ScriptToken tok(pos, ScriptTokenType::FUNCTION_NAME, word);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(word.size());
	return tok;
      }
    }
    else if (ctx.isLastLevelAny(NestLevel::SCRIPT_CONDITION)){
      if (ctx.isLastTokenType(ScriptTokenType::CONDITION_OPENING)){
	return getRValueToken(ctx);
      }
      
      if (ctx.isLastTokenClass(TokenClass::RVALUE_TOKEN) && word == "do"){
	if (ctx.isLastLevel(NestLevel::SCRIPT_IF_CONDITION)){
	  ScriptToken tok(pos, ScriptTokenType::IF_CONDITION_BODY_OPENING);
	  ctx.popLastNestLevel();
	  ctx.pushNestLevel(NestLevel::SCRIPT_IF_BODY);
	  ctx.getFilePointerRef().advance(word.size());
	  return tok;
	}else if (ctx.isLastLevel(NestLevel::SCRIPT_WHILE_CONDITION)){
	  ScriptToken tok(pos, ScriptTokenType::WHILE_CONDITION_BODY_OPENING);
	  ctx.popLastNestLevel();
	  ctx.pushNestLevel(NestLevel::SCRIPT_WHILE_BODY);
	  ctx.getFilePointerRef().advance(word.size());
	  return tok;
	}
        throw GeneralException(A_TOKENIZER_ERROR_17);
      }
      return getRValueToken(ctx);
    }
    else if (ctx.isLastLevel(NestLevel::SCRIPT_NAME_DECLARATION)){
      if (c == '=' && ctx.isLastTokenType(ScriptTokenType::VARIABLE_NAME)){
	ScriptToken tok(pos, ScriptTokenType::DEFINITION_EQUAL);
	ctx.setLastToken(tok);
	ctx.popLastNestLevel();
	ctx.getFilePointerRef().advance();
	return tok;
      }
      getPastWhiteSpace(tmp);
      if (tmp.getChar() == '='){
	ScriptToken tok(pos, ScriptTokenType::VARIABLE_NAME, word);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(word.size());
	return tok;
      }
      else if (ctx.isLastTokenType(ScriptTokenType::DECLARATION_LET) ||
	       ctx.isLastTokenType(ScriptTokenType::QUALIFIER) ||
	       ctx.isLastTokenType(ScriptTokenType::TYPE_DELIMETER) ||
	       ctx.isLastTokenType(ScriptTokenType::LIST_ELEMENTS_TYPE_OPENING)) {
	ScriptToken tok(pos, ScriptTokenType::TYPE_NAME, word);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(word.size());
	return tok;
      }
      else if (ctx.isLastTokenType(ScriptTokenType::TYPE_NAME)){
	throw GeneralException(A_TOKENIZER_ERROR_13);
      }
      throw GeneralException(A_TOKENIZER_ERROR_16);
    }
    else if (ctx.isLastLevel(NestLevel::SCRIPT_FUNCTION_PARAMETER_LIST)){
      if (c == ')' || c == ','){
	if (c == ',' && ctx.isLastTokenType(ScriptTokenType::FUNCTION_PARAMETER_NAME)){
	  ScriptToken tok(pos, ScriptTokenType::FUNCTION_PARAMETERS_COMMA);
	  ctx.setLastToken(tok);
	  ctx.getFilePointerRef().advance();
	  return tok;
	}
	else if (c == ')' && (ctx.isLastTokenType(ScriptTokenType::FUNCTION_PARAMETER_NAME)||ctx.isLastTokenType(ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_OPENING))) {
	  ScriptToken tok(pos, ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_CLOSING);
	  ctx.setLastToken(tok);
	  ctx.getFilePointerRef().advance();
	  ctx.popLastNestLevel();
	  return tok;
	}else{
	  throw GeneralException(A_TOKENIZER_ERROR_16 + ": '" + std::string(1, c) + "'");
	}
      }
	
	
      getPastWhiteSpace(tmp);
      if (!word.empty() && (tmp.getChar() == ',' || tmp.getChar() == ')')){
	ScriptToken tok(pos, ScriptTokenType::FUNCTION_PARAMETER_NAME, word);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(word.size());
	return tok;
      }
      else if (ctx.isLastTokenType(ScriptTokenType::FUNCTION_DECLARATION_PARAMETER_LIST_OPENING) ||
	       ctx.isLastTokenType(ScriptTokenType::FUNCTION_PARAMETERS_TYPES_COMMA) ||
	       ctx.isLastTokenType(ScriptTokenType::QUALIFIER) ||
	       ctx.isLastTokenType(ScriptTokenType::TYPE_DELIMETER)){
	ScriptToken tok(pos, ScriptTokenType::TYPE_NAME, word);
	ctx.setLastToken(tok);
	ctx.getFilePointerRef().advance(word.size());
	return tok;	
      }else{
	throw GeneralException(A_TOKENIZER_ERROR_16);
      }
    }
    throw GeneralException(A_TOKENIZER_ERROR_16);
  }
  
  Token Tokenizer::getNextRValueToken(SyntacticContext &ctx)
  {
    if (!ctx.isLastTokenClass(TokenClass::RVALUE_TOKEN)){
      return getValueToken(ctx);
    }
    else if (ctx.isLastLevel(NestLevel::RVALUE_TEXT_DOUBLE_QUOTED)||
	     ctx.isLastLevel(NestLevel::RVALUE_TEXT_SINGLE_QUOTED)){
      
      char c = ctx.getFilePointerRef().readChar();

      if (c == '"' || c == '\''){
	return getTextClosingToken(ctx);
      }else{
	return getTextContentToken(ctx);
      }
    } 
    else if (ctx.isLastTokenType(RValueTokenType::VALUE)){
      getPastWhiteSpace(ctx.getFilePointerRef());
      char c = ctx.getFilePointer().readChar();

      if (c == '[' && (ctx.isLastTokenType(RValueTokenType::LIST_CLOSING) || ctx.isLastTokenType(RValueTokenType::TUPLE_CLOSING))){
	return getContainerIndexToken(ctx);
      }
      else if (c == '[' && ctx.isLastTokenType(RValueTokenType::IDENTIFIER)){
	// look ahead to see if id component
	FilePointer tmp = ctx.getFilePointer();

	// if there is an id name, and a single equal afterwards
	if (!getName(tmp).empty() && getNextNonSpaceChar(tmp) == '=' && getNextNonSpaceChar(tmp) != '='){
	  return getIdComponentOpeningToken(ctx);
	}else{
	  return getContainerIndexToken(ctx);
	}
      }
      else if (c == '(' && (ctx.isLastTokenType(RValueTokenType::LAMBDA_CLOSING) || ctx.isLastTokenType(RValueTokenType::IDENTIFIER))){
	return getFunctionCallOpeningToken(ctx);
      }
      else if (c == '.' && (ctx.isLastTokenType(RValueTokenType::DICTIONARY_CLOSING) || ctx.isLastTokenType(RValueTokenType::IDENTIFIER))){
	return getMemberOperatorToken(ctx);
      }
      else if (c != '\n' && doesOperatorFollow(ctx)){
	return getInfixOperatorToken(ctx);
      }else{
        ctx.popLastNestLevel();
	return getNextToken(ctx); 
      }
    }
    else if (ctx.isLastTokenType(RValueTokenType::INFIX_OPERATOR)){
      return getValueToken(ctx);
    }
    else if (ctx.isLastTokenType(RValueTokenType::DICTIONARY_OPENING)){
      throw GeneralException(A_TOKENIZER_ERROR_13);
    }
    throw GeneralException(A_TOKENIZER_ERROR_16);
  }
  
  void Tokenizer::getPastWhiteSpace(FilePointer &fp)
  {
    char c = fp.readChar();
    while (c == ' ' || c == '\t'){
      fp.advance();
      c = fp.readChar();
    }
  }

  void Tokenizer::getPastWhiteSpaceAndLineBreaks(FilePointer &fp)
  {
    char c = fp.readChar();
    while (c == ' ' || c == '\t' || c == '\n'){
      fp.advance();
      c = fp.readChar();
    }
  }
  
  const std::string Tokenizer::getWord(FilePointer &fp)
  {
    unsigned nameLength = 0;
    char c = fp.readChar();
    
    c = fp.readChar(nameLength);
    while (c != ' ' && c != '\n' && c != '\t' && c != '\0'){
      nameLength ++;
      c = fp.readChar(nameLength);
    }
    return fp.getSubstring(nameLength);
  };

  const std::string Tokenizer::getName(FilePointer &fp)
  {
    unsigned nameLength = 0;
    char c = fp.readChar();
    
    c = fp.readChar(nameLength);
    while (c != '\0' && (c == '_' || isalpha(c) || (nameLength > 0 && isdigit(c)))){
      nameLength ++;
      c = fp.readChar(nameLength);
    }
    return fp.getSubstring(nameLength);
  };

  char Tokenizer::getNextNonSpaceChar(A::FilePointer &fp)
  {
    getPastWhiteSpace(fp);
    return fp.getChar();
  }  
}
