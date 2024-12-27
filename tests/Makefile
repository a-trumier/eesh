CXX=g++
CXXFLAGS=-g -Wall -Wextra -I../include -std=c++17 -O2 
LDFLAGS=

.PHONY=all clean

SOURCES := $(wildcard *.cpp)
# Binaries (remove .cpp extension)
BINARIES := $(SOURCES:.cpp=)

.PHONY: all clean

# Default target: build all binaries
all: build/input.o build/read_input_test

build/input.o: ../src/input.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/read_input_test: build/read_input_test.o
	$(CXX) $(LDFLAGS) -o $@ $< build/input.o

build/read_input_test.o: read_input_test.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up binaries
clean:
	rm -rf build
