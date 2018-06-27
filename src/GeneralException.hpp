#ifndef A_GENERAL_EXCEPTION
#define A_GENERAL_EXCEPTION

#include "BaseException.hpp"

#include <string>

namespace A
{
  class GeneralException : public BaseException
  {
  public:
    GeneralException(std::string str):BaseException(), _exceptionString(str){}
    
    virtual const std::string getExceptionString() const
    {
      return _exceptionString;
    }
    
  private:
    std::string _exceptionString;
  };
}

#endif // A_GENERAL_EXCEPTION
