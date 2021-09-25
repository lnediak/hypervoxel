all: main

main: main.o ../lodepng/lodepng.o
	g++ $+ -lOpenCL -o $@

../lodepng/lodepng.o: ../lodepng/lodepng.cpp ../lodepng/lodepng.h
	g++ -I../lodepng -c -o $@ $<

main.o: main.cpp ../*.hpp ../cl/cl_sources_all.inc
	g++ --std=c++11 -Wall -Wextra -pedantic -g3 -O2 -I.. -c -o $@ $<

../cl/cl_sources_all.inc: ../cl/cl_sources_all.cl
	xxd -i $< > $@

../cl/cl_sources_all.cl: ../cl/util.cl ../cl/terrain_indexer.cl ../cl/lerp_terrain.cl ../cl/render_terrain.cl
	cat $^ > $@

clean:
	/bin/rm -f main main.o

