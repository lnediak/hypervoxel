all: test_cl

test_cl: test_cl.o lodepng/lodepng.o
	g++ -o test_cl test_cl.o lodepng/lodepng.o -lOpenCL

lodepng/lodepng.o: lodepng/lodepng.cpp lodepng/lodepng.h
	g++ -g3 -O2 -c -o lodepng/lodepng.o lodepng/lodepng.cpp

test_cl.o: test_cl.cpp load_cl.hpp main.cl.inc
	g++ --std=c++11 -Wall -Wextra -pedantic -g3 -O2 -I. -c -o test_cl.o test_cl.cpp

main.cl.inc: main.cl.pp
	../preprocess/pp main.cl.pp 3 12 > main.cl.inc

.PHONY: clean

clean:
	rm main.cl.inc test_cl.o lodepng/lodepng.o test_cl

