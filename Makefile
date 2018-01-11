SHELL:=/bin/bash

LIBS = -L../Halide/cmakebuild/lib -lHalide -lpng -lz -ldl -lpthread -lncurses -ljpeg
INCLUDES = -I../Halide/cmakebuild/include -I../Halide/tools
CXXFLAGS = -std=c++14

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) hafast.cc -o hafast $(LIBS)
