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
    CascadedException(std::string str):BaseException(), _exceptionStrings({str}){}

    CascadedException(std::vector<const std::string> strs):BaseException(), _exceptionStrings(strs){}

    CascadedException(const CascadedException &cascadedException, std::string str):BaseException(), _exceptionStrings(cascadedException.getExceptionStrings())
    {
      _exceptionStrings.push_back(str);
    }
    
    virtual const std::string getExceptionString() const
    {
      std::string indent = "";
      std::string out = "Error:\n";
      for (auto &s : _exceptionStrings){
	out += indent + s + "\n";
	indent += "--";
      }
      return out;
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
