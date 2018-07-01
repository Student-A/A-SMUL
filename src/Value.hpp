#ifndef A_VALUE_H
#define A_VALUE_H

#include "EnumBase.hpp"

#include <variant>
#include <string>
#include <vector>
#include <map>

namespace A
{
  enum class ValueType : unsigned
  {
    NONE = BIT(0),
    NUMBER = BIT(1),
    LIST = BIT(2),
    SWITCH = BIT(3),
    DICTIONARY = BIT(4),
    TEXT = BIT(5),

    VARIANT = NONE | NUMBER | LIST | SWITCH | DICTIONARY | TEXT,
  };
  ENUM_FLAG_BIN_OPERATOR(ValueType, &)
  ENUM_FLAG_BIN_OPERATOR(ValueType, |)
  ENUM_FLAG_UN_OPERATOR(ValueType, ~)
  
  class Value
  {
  public:
    Value(void *data, ValueType type) : _type(type)
    {
      switch (type){
      case ValueType::NONE:
	break;
      case ValueType::NUMBER:
	_value = static_cast<double>(*data);
	break;
      case Value::TEXT:
	_value = static_cast<std::string>(*data);
	break;
      case ValueType::SWITCH:
	_value = static_cast<bool>(*data);
	break;
      default:
	//[throw wrong type]
	;
      }
    }

    void set()
    {
      _valueType = ValueType::NONE;
      if ((_valueType & _type) != _valueType){
	//[throw wrong type]
      }
    }
    
    void set(double d)
    {
      _valueType = ValueType::NUMBER;
      if ((_valueType & _type) != _valueType){
	//[throw wrong type]
      }
      _value = d;
    }

    void set(bool b)
    {
      _valueType = ValueType::SWITCH;
      if ((_valueType & _type) != _valueType){
	//[throw wrong type]
      }
      _value = b;
    }

    void set(std::string s)
    {
      _valueType = ValueType::TEXT;
      if ((_valueType & _type) != _valueType){
	//[throw wrong type]
      }
      _value = s;
    }

    double get()
    {
      if ((ValueType::NUMBER & _type) != ValueType::NUMBER){
	//[throw wrong type]
      }
      return std::get<double>(_value);
    }

    bool get()
    {
      if ((ValueType::SWITCH & _type) != ValueType::SWITCH){
	//[throw wrong type]
      }
      return std::get<bool>(_value);
    }

    std::string get()
    {
      if ((ValueType::TEXT & _type) != ValueType::TEXT){
	//[throw wrong type]
      }
      return std::get<std::string>(_value);
    }

    bool hasTypes(unsigned)
    {
    }
    
  private:
    std::variant<std::string,
		 double,
		 std::vector<Value*>,
		 bool> _value;

    ValueType _type{NONE};
    ValueType _valueType{NONE};
  };
}

#endif // A_VALUE_H
