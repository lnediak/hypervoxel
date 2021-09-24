all: main

main: main.cpp ../*.hpp
	g++ --std=c++11 -Wall -Wextra -pedantic -g3 -O2 -I.. -o $@ $<

clean:
	/bin/rm -f main

