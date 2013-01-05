CCX = clang++
CXXFLAGS = -stdlib=lbc++ -Wall -O3
OBJECTS = test.o


test : test/test.cpp
	$(CCX) $(CCXFLAGS) -o test -I../googletest/include
