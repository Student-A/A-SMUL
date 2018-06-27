#include "Tag.hpp"
#include "GeneralException.hpp"

namespace A
{
  Tag::Tag()
  {
    // ctor
  }

  Tag::Tag(std::string name, std::vector<RValueToken> parameter) : _name(name), _parameters(parameter)
  {
  }

  Tag::Tag(std::string name, std::vector<std::vector<RValueToken>> parameters) : _name(name)
  {
    _parameters = parameters;
  }

  std::string Tag::getName() const
  {
    return _name;
  }
  
  void Tag::addParameter(std::vector<RValueToken> parameter)
  {
    _parameters.push_back(parameter);
  }

  void Tag::addParameters(std::vector<std::vector<RValueToken>> parameters)
  {
    _parameters.insert(std::end(_parameters), parameters);
  }

  unsigned getParameterCount() const
  {
    return _parameters.size();
  }
  
  std::vector<RValueToken> &Tag::operator() (int parameterIndex) {
    if (parameterIndex >= _parameters.size()) {
      throw GeneralException("tag: '" + _tagName + "' requires " + stringify(parameterIndex) + " argument(s)");
    }
    if (parameterIndex == -1)
      return _parameters.back();
    return _parameters.at(parameterIndex);
  }
}
