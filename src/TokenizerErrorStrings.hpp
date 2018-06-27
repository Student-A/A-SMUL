#ifndef A_TOKENIZER_ERROR_STRINGS_HPP
#define A_TOKENIZER_ERROR_STRINGS_HPP

#define A_TOKENIZER_ERROR_0 std::string("Invalid nest level in context for a \"Markup token\"")

#define A_TOKENIZER_ERROR_1 std::string("Invalid previous token for nest level \"MARKUP_ID_COMPONENT_KEY\"")

#define A_TOKENIZER_ERROR_2 std::string("Expected a name for main component after '{'")

#define A_TOKENIZER_ERROR_3 std::string("Expected '}' after the main component name")

#define A_TOKENIZER_ERROR_4 std::string("Expected '=' after id component name")

#define A_TOKENIZER_ERROR_5 std::string("Expected '[' to mark the sub component opening")

#define A_TOKENIZER_ERROR_6 std::string("Expected '/' to mark the id component closing")

#define A_TOKENIZER_ERROR_7 std::string("Found an id component closing token, but its name does not match with opening")

#define A_TOKENIZER_ERROR_8 std::string("Expected ']' to mark the id component closing")

#define A_TOKENIZER_ERROR_9 std::string("Expected '/' to mark the main component closing")

#define A_TOKENIZER_ERROR_10 std::string("Found a main component closing token, but its name does not match with opening")

#define A_TOKENIZER_ERROR_11 std::string("Expected '=' after tag name")

#define A_TOKENIZER_ERROR_12 std::string("Expected ':' after switch name")

#define A_TOKENIZER_ERROR_13 std::string("Not yet implemented")

#define A_TOKENIZER_ERROR_14 std::string("Invalid token for nest level")

#define A_TOKENIZER_ERROR_15 std::string("Invalid infix operator token")

#define A_TOKENIZER_ERROR_16 std::string("Token does not match in this context")

#define A_TOKENIZER_ERROR_17 std::string("Bug")
  
#endif // A_TOKENIZER_ERROR_STRINGS_HPP
