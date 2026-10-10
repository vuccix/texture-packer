#include "TexturePacking.h"
#include <Image/Image.h>
#include <IO/IO.h>
#include <omp.h>
#include <cassert>
#include <vector>
#include <mdspan>

namespace {

constexpr uint32_t RED   = 0;
constexpr uint32_t GREEN = 1;
constexpr uint32_t BLUE  = 2;
constexpr uint32_t ALPHA = 3;

void roughnessToSmoothness(Image& image) {
    const std::mdspan pixels(image.pixels.data(), image.height, image.width);

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height; ++y)
        for (uint32_t x = 0; x < image.width; ++x)
            pixels[y, x].a = static_cast<uint8_t>(255 - pixels[y, x].r);
}

void packChannel(Image& image, const Image& texture, const uint32_t channel) {
    const std::mdspan pixels(image.pixels.data(), image.height, image.width);
    const std::mdspan tex(texture.pixels.data(), texture.height, texture.width);

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height; ++y)
        for (uint32_t x = 0; x < image.width; ++x)
            pixels[y, x][channel] = tex[y, x][channel];
}

Image unity(Bundle& b) {
    constexpr Pixel defaultPixel = {
        .r = 0,   // metallic
        .g = 255, // occlusion
        .b = 0,   // not used
        .a = 127, // smoothness
    };

    Image result = {
        .pixels  = std::vector(b.width * b.height, defaultPixel),
        .width   = b.width,
        .height  = b.height,
    };

    if (b.metallic.hasTex())
        ::packChannel(result, b.metallic, ::RED);   // metallic

    if (b.occlusion.hasTex())
        ::packChannel(result, b.occlusion, ::GREEN); // occlusion

    if (b.roughness.hasTex()) {
        ::roughnessToSmoothness(b.roughness);
        ::packChannel(result, b.roughness, ::ALPHA); // smoothness
    }

    return result;
}

Image orm(const Bundle& b) {
    constexpr Pixel defaultPixel = {
        .r = 255, // occlusion
        .g = 127, // roughness
        .b = 0,   // metallic
        .a = 255, // not used
    };

    Image result = {
        .pixels  = std::vector(b.width * b.height, defaultPixel),
        .width   = b.width,
        .height  = b.height,
    };

    if (b.occlusion.hasTex()) ::packChannel(result, b.occlusion, ::RED);   // occlusion
    if (b.roughness.hasTex()) ::packChannel(result, b.roughness, ::GREEN); // roughness
    if (b.metallic.hasTex())  ::packChannel(result, b.metallic,  ::BLUE);  // metallic

    return result;
}

}

void Pack::process(Bundle& bundle, const fs::path& outPath, const Format format) {
    assert(bundle.width > 0 && bundle.height > 0);
    assert(outPath.empty() == false);

    const Image result = (format == Format::Unity) ? ::unity(bundle) : ::orm(bundle);

    IO::save(result, outPath);
}
