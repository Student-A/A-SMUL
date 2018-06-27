#ifndef A_BASE_EXCEPTION
#define A_BASE_EXCEPTION

#include <exception>
#include <string>

namespace A
{
  class BaseException : public std::exception
  {
  public:
    
    const char* what() const throw()
    {
      return getExceptionString().c_str();
    }

    virtual const std::string getExceptionString() const = 0;
  };
}

#endif // A_BASE_EXCEPTION
