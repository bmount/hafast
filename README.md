
# HaFAST

Halide experiment in implementing the
["Features from accelerated segment test (FAST)"](https://en.wikipedia.org/wiki/Features_from_accelerated_segment_test)
algorithm for corner-y image features. Part of the exercise is seeing whether the initial rejection test can
be omitted (trade extra compute for better memory locality), and whether some tile-level reduction could do a simple and
cheap form of non-maximal suppression.

### Build

Run `make`, after extracting the output of Halide's `make distrib` target into project root.

### License: MIT

Feel free to use for any purpose -- free as in couch.
