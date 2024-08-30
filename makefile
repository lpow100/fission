CXX=g++
DEPS = lexer.hpp
OBJ = lexer.o
BUILD_MODE = DEFUALT
TEST = FALSE
ifeq ($(BUILD_MODE),DEBUG)
	CXXFLAGS += -g
endif
ifeq ($(TEST),TRUE)
	OBJ += test.o
else
	OBJ += main.o
endif

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	del *.o *.exe /s