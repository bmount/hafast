#include "Halide.h"
#include "halide_image_io.h"
#include <stdint.h>

namespace ha = Halide;

using Halide::cast;
using Halide::abs;
using Halide::Var;
using Halide::Func;

int main() {
    ha::Buffer<uint8_t> img = ha::Tools::load_image("images/large.png");
    Func gray("gray"), keypoints("keypoints"), visuals("visuals");
    Var x("x"), y("y"), c("c");
    ha::Expr input = img(x, y);
    input = ha::cast<float>(input);
    input = img(x, y, 0) * 0.299f + img(x, y, 1) * 0.587f + img(x, y, 2) * 0.114f;
    int br = 3; // Destination buffer border, Bresenham radius for ring of 16 surrounding pixels:
    float tr = 60.f;
    gray(x, y) = input;

    keypoints(x, y) =
            cast<float>(abs(gray(x,     y - 3) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 1, y - 3) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 2, y - 2) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 3, y - 1) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 3, y)     - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 3, y + 1) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 2, y + 2) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x + 1, y + 3) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x,     y + 3) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 1, y + 3) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 2, y + 2) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 3, y + 1) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 3, y)     - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 3, y - 1) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 2, y - 2) - gray(x, y)) > tr) +
            cast<float>(abs(gray(x - 1, y - 3) - gray(x, y)) > tr)
        >
            12.f; // 12 of 16 neighbors are sharp transition, value from paper

    visuals(x, y) = ha::cast<uint8_t>(ha::min(keypoints(x, y) * 120.f, 255.f));

    ha::Buffer<uint8_t> output(img.width() - br * 2, img.height() - br * 2);
    output.set_min(br, br);

    ha::Var block, thread, i, xo, yo, xi, yi;
    visuals.gpu_tile(x, y, xo, yo, xi, yi, 8, 8);
    keypoints.compute_at(visuals, xo);
    keypoints.gpu_threads(x, y);

    ha::Target target = ha::get_host_target();
    target.set_feature(ha::Target::OpenCL);
    visuals.compile_jit(target);

    //visuals.print_loop_nest();
    //visuals.compile_to_lowered_stmt("keypoints.html", {}, ha::HTML);
    //visuals.compile_to_c("k.cpp", {}, "keypoints");
    using namespace std::chrono;
    auto t0 = steady_clock::now();
    visuals.realize(output);
    auto t = duration_cast<milliseconds>(steady_clock::now() - t0).count();
    std::cerr << "Time: " << t << "\n";

    ha::Tools::save_image(output, "output.png");
    return 0;
}
