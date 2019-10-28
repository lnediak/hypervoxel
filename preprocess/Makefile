all: pp

pp: preprocess.cpp
	g++ --std=c++11 -Wall -Wextra -pedantic -g3 -O2 -o pp preprocess.cpp

.PHONY: clean

clean:
	rm pp

