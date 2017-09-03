CXX = clang++
FLAGS = -std=c++1z -Wunreachable-code -Wall
INCLUDES = -I${CURDIR}/src

default: tokenizer_test

debug: FLAGS += -g
debug: tokenizer_test

tokenizer_test: Tokenizer.o TokenTypes.o TokenizerTest.o 
	$(CXX) $(FLAGS) -o testbins/TokenizerTest Tokenizer.o TokenTypes.o TokenizerTest.o
	rm -f *.o

Tokenizer.o:
	$(CXX) $(FLAGS) $(INCLUDES) -c src/Tokenizer.cpp

TokenTypes.o:
	$(CXX) $(FLAGS) $(INCLUDES) -c src/TokenTypes.cpp

TokenizerTest.o:
	$(CXX) $(FLAGS) $(INCLUDES) -c tests/TokenizerTest.cpp

clean:
	rm -f *.o testbins/*
