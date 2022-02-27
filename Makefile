CXX ?= g++
CXXFLAGS ?=  -o $@ -O2

matrix: main.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $<

gentest: ./Tests/gentest.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $<
