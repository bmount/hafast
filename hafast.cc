#include "Halide.h"
#include "halide_image_io.h"
#include <stdint.h>

namespace ha = Halide;

int main() {
    ha::Buffer<uint8_t> img = ha::Tools::load_image("images/large.png");
    ha::Func gray;
    ha::Var x("x"), y("y"), c("c");
    ha::Expr input = img(x, y);
    input = ha::cast<float>(input);
    input = img(x, y, 0) * 0.299f + img(x, y, 1) * 0.587f + img(x, y, 2) * 0.114f;
    input = ha::cast<uint8_t>(input);
    gray(x, y) = input;

    ha::Buffer<uint8_t> output = gray.realize(img.width(), img.height());

    ha::Tools::save_image(output, "output.png");
    return 0;
}
