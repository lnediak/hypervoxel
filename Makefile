GPROF =
CXX = g++ -std=c++11 -Wall -Wextra -pedantic -g3 -O2 -ftemplate-backtrace-limit=0 $(GPROF)

all: basic_test

basic_test: basic_test.o
	g++ -o $@ $< -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor $(GPROF)

basic_test.o: basic_test.cpp *.hpp
	$(CXX) -c -o $@ $<

chtbl_test: chtbl_test.cpp concurrent_hashtable.hpp
	$(CXX) -o $@ $< -lpthread

clean:
	/bin/rm basic_test.o basic_test chtbl_test

