CXX=g++

CPP_SOURCES=$(shell find . -name "*.cpp")
OBJS=$(patsubst ./%.cpp,./build/%.o,$(CPP_SOURCES))

.PHONY=all test clean

all : ./build/oper
	./build/oper test.op

./build/oper: $(OBJS)
	$(CXX) -o build/oper $^

build/%.o : %.cpp
	@mkdir -p build
	$(CXX) -g -Wall -std=c++2a -o $@ -c $<

test : ./build/oper
	python3 tests.py

clean:
	rm -rf build
	
	
