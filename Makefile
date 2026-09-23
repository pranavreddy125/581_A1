CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror

.PHONY: all test clean

all: build/ipv4_extractor

build:
	mkdir -p build

build/ipv4_extractor: main.cpp ipv4.cpp ipv4.hpp | build
	$(CXX) $(CXXFLAGS) main.cpp ipv4.cpp -o $@

build/test_ipv4: tests/test_ipv4.cpp ipv4.cpp ipv4.hpp | build
	$(CXX) $(CXXFLAGS) tests/test_ipv4.cpp ipv4.cpp -o $@

test: build/ipv4_extractor build/test_ipv4
	./build/test_ipv4
	python3 tests/test_cli.py ./build/ipv4_extractor

clean:
	rm -rf build
