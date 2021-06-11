GPROF =
CXX = g++ -std=c++11 -Wall -Wextra -pedantic -g3 -O2 -ftemplate-backtrace-limit=0 $(GPROF)

all: basic_test

basic_test: basic_test.o
	g++ -o $@ $< -lOpenCL -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lXi -ldl -lXinerama -lXcursor $(GPROF)

basic_test.o: basic_test.cpp *.hpp cl/cl_sources_all.inc
	$(CXX) -c -o $@ $<

cl/cl_sources_all.inc: cl/cl_sources_all.cl
	xxd -i $< > $@

cl/cl_sources_all.cl: cl/util.cl cl/terrain_indexer.cl cl/terrain_generator.cl cl/structs.cl cl/follow_line.cl cl/kernel.cl 
	cat $^ > $@

clean:
	/bin/rm basic_test.o basic_test cl/cl_sources_all.cl cl/cl_sources_all.inc

