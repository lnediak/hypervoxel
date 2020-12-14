all: basic_test

basic_test: basic_test.o
	g++ basic_test.o -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -o basic_test

basic_test.o: basic_test.cpp
	g++ basic_test.cpp -c -o basic_test.o -ftemplate-backtrace-limit=0 -std=c++11 -Wall -Wextra -pedantic -g3 -O2

clean:
	/bin/rm basic_test.o basic_test

