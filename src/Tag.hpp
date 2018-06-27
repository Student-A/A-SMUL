#ifndef A_TAG_HPP
#define A_TAG_HPP

#include <string>
#include <vector>
#include "Token.hpp"

namespace A
{
  class Tag
  {
  public:
    Tag(std::string name);

    Tag();

    unsigned getParameterCount() const;

    std::string getName() const;
    
    void addParameter(std::vector<RValueToken> valueExpression);

    std::vector<RValueToken> &operator()(int parameterIndex);

  private:
    std::string _name;

    std::vector<std::vector<RValueToken>> _parameters; 
  };
}

#endif // A_TAG_HPP
