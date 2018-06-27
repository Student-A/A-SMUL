#include <cstdint>


#define BIT(x) (std::uint64_t(1) << std::uint64_t(x))

#define BIT_TEST(x, b) ((x & b) == b)

#define BIT_ANY(x, b) ((x & b) != 0)

#define BIT_TOGGLE(x, b) (x ^ b)

#define BIT_ON(x, b) (x | b)

#define BIT_OFF(x, b) (x & (~b))

#define ENUM_FLAG_BIN_OPERATOR(T, O) inline T operator O (T lhs, T rhs) {return static_cast<T>(static_cast<std::underlying_type_t <T>>(lhs) O static_cast<std::underlying_type_t <T>>(rhs));} 

#define ENUM_FLAG_UN_OPERATOR(T, O) inline T operator O (T x) {return static_cast<T>(O static_cast<std::underlying_type_t <T>>(x));}
