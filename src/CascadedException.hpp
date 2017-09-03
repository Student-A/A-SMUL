#ifndef A_CASCADED_EXCEPTION
#define A_CASCADED_EXCEPTION

#include "BaseException.hpp"

#include <string>
#include <vector>


namespace A
{
  class CascadedException : public BaseException
  {
  public:
    virtual const std::string getExceptionString() const
    {
      std::string indent = "";
      std::string out = "Error:\n";
      for (auto &s : _exceptionStrings){
	out += indent + s + "\n";
	indent += "--";
      }
      return _exceptionString;
    }

    const std::vector<const std::string> &getExceptionStrings() const
    {
      return _exceptionStrings;
    }

    const std::string &getLastExceptionString()
    {
      return _exceptionStrings.back();
    }
    
  private:
    std::vector<const std::string> _exceptionStrings;
  };
}

#endif // A_CASCADED_EXCEPTION
