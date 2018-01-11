SHELL:=/bin/bash

# run `make distrib` from within Halide to generate an archive, place at <project root>/halide
LIBS = -L./halide/lib -lHalide -lpng -lz -ldl -lpthread -lncurses -ljpeg
INCLUDES = -I./halide/include -I./halide/tools
CXXFLAGS = -std=c++14

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) hafast.cc -o hafast $(LIBS)
