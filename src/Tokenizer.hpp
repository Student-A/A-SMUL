#ifndef A_TOKENIZER_HPP
#define A_TOKENIZER_HPP

#include "SyntacticContext.hpp"
#include "FilePointer.hpp"
#include "Token.hpp"

namespace A
{
  class Tokenizer
  {
  public:
    Tokenizer(const std::string *source);
    
    Tokenizer(const std::string *source, SyntacticContext context);

    ~Tokenizer();
    
    Token getNextToken();

    Token getNextToken(SyntacticContext &context);

  private:
    const std::string *_source;
    SyntacticContext _context;
    
    void getPastWhiteSpace(FilePointer &fp);

    void getPastWhiteSpaceAndLineBreaks(FilePointer &fp);

    Token getNextScriptToken(SyntacticContext &ctx);

    Token getNextMarkupToken(SyntacticContext &ctx);

    Token getNextRValueToken(SyntacticContext &ctx);

    Token getMarkupCharacterDependentToken(SyntacticContext &ctx);
    
    Token getMainComponentOpeningNameToken(SyntacticContext &ctx);

    Token getMainComponentOpeningNameClosingToken(SyntacticContext &ctx);

    Token getIdComponentIdNameToken(SyntacticContext &ctx);

    Token getIdComponentIdEqualToken(SyntacticContext &ctx);

    Token getIdComponentIdClosingToken(SyntacticContext &ctx);
    
    Token getSubComponentBracketToken(SyntacticContext &ctx);

    Token getSubComponentClosingToken(SyntacticContext &ctx);
    
    Token getTagEqualToken(SyntacticContext &ctx);

    Token getSwitchColonToken(SyntacticContext &ctx);

    Token getIdComponentOpeningToken(SyntacticContext &ctx);

    Token getIdComponentClosingToken(SyntacticContext &ctx);

    Token getIdComponentClosingSlashToken(SyntacticContext &ctx);

    Token getIdComponentClosingIdNameToken(SyntacticContext &ctx);

    Token getIdComponentClosingIdNameClosingToken(SyntacticContext &ctx);
    
    Token getMainComponentOpeningToken(SyntacticContext &ctx);

    Token getMainComponentClosingToken(SyntacticContext &ctx);

    Token getMainComponentClosingSlashToken(SyntacticContext &ctx);

    Token getMainComponentClosingNameToken(SyntacticContext &ctx);

    Token getMainComponentClosingNameClosingToken(SyntacticContext &ctx);
    
    Token getCommentToken(SyntacticContext &ctx);

    Token getMultilineCommentOpeningToken(SyntacticContext &ctx);

    Token getContainerIndexToken(SyntacticContext &ctx);

    Token getFunctionCallOpeningToken(SyntacticContext &ctx);

    Token getMemberOperatorToken(SyntacticContext &ctx);

    Token getInfixOperatorToken(SyntacticContext &ctx);

    Token getValueToken(SyntacticContext &ctx);

    Token getTextContentToken(SyntacticContext &ctx);

    Token getTextClosingToken(SyntacticContext &ctx);

    Token getConditionBodyClosingToken(SyntacticContext &ctx);
    
    Token getRValueToken(SyntacticContext &ctx);
    
    // SCRIPT
    Token getScriptCommandToken(SyntacticContext &ctx);
    Token getScriptCommandToken(SyntacticContext &ctx, std::string &lookaheadWord, FilePointer &pos);
    
    Token getUndefinedToken(SyntacticContext &ctx);

    // General
    const std::string getName(FilePointer &fp);

    const std::string getWord(FilePointer &fp);

    char getNextNonSpaceChar(FilePointer &fp);
    
    bool doesOperatorFollow(SyntacticContext &ctx);
  };
}

#endif // A_TOKENIZER_HPP
