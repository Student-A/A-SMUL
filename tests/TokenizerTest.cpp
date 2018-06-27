#include <iostream>
#include <string>
#include "FilePointer.hpp"
#include "Token.hpp"
#include "Tokenizer.hpp"

int main()
{
  std::string afile = R"(
let number a = 1
if a == 5 do
 a = 2
end
{maincomp}
  [hello = 
1]
    tag = 1 + 1
  [/hello]

comp3[ tag1 = "123\"123" ]
{/maincomp}
{com2}
{/com2}

)";

  A::FilePointer fp(afile);

  A::Tokenizer tokenizer(&afile);

  A::Token tok = tokenizer.getNextToken();

  for (unsigned i = 0; i < 100 && tok.getTokenType() != 0; i ++){
    std::cout << tok.toString() << std::endl;
    tok = tokenizer.getNextToken();
  }
  
  return 0;
}
