CXX = g++
CXXFLAGS = -o $@

matrix: main.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $<

gentest: ./Tests/gentest.cpp matrix.hpp
	$(CXX) $(CXXFLAGS) $<
