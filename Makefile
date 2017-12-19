SHELL:=/bin/bash

LIBS = -L./halide/lib -lHalide -lpng -lz -ldl -lpthread -lncurses -ljpeg
INCLUDES = -I./halide/include -I./halide/tools
CXXFLAGS = -std=c++14 -O3

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) hafast.cc -o hafast $(LIBS)
