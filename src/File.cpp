#include "File.hpp"

#include "FilePointer.hpp"
#include "Tokenizer.hpp"

#include <fstream>

namespace A
{
  File::File(std::string fileDir, std::string callerDir)
  {
    auto f = std::ifstream(fileDir);
    std::string content((std::istreambuf_iterator<char>(f)),
			(std::istreambuf_iterator<char>()));

    FilePointer fp(content);

    Tokenizer tokenizer(&fp);

    Token tok = tokenizer.getNextToken();
    
   
    if (tok.isTokenClass(TokenClass::MARKUP_TOKEN)){
      if (tok.isTokenType(MarkupTokenType::MAIN_COMPONENT_OPENING)){
        auto component = parseMainComponent(tokenizer);
	addMainComponent(component);
	tok.get
      }
    }
  }

  std::shared_ptr<MainComponent> parseMainComponent(Tokenizer &tokenizer)
  {
    Token tok = tokenizer.getNextToken();
    std::string name = tok.getTokenString();
    (void)tokenizer.getNextToken();
    auto component = parseComponent(tokenizer);
    (void)tokenizer.getNextToken();
    (void)tokenizer.getNextToken();
    (void)tokenizer.getNextToken(); // end
    return MainComponent(component, name);
  }

  std::shared_ptr<Component> parseComponent(Tokenizer &tokenizer)
  {
    
  }
}
