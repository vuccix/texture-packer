#include "TexturePacking.h"
#include <Image/Image.h>
#include <IO/IO.h>
#include <omp.h>
#include <cassert>
#include <vector>

namespace {

constexpr uint32_t RED   = 0;
constexpr uint32_t GREEN = 1;
constexpr uint32_t BLUE  = 2;
constexpr uint32_t ALPHA = 3;

void roughnessToSmoothness(Image& image) {
    auto& pixels = image.pixels;

    #pragma omp parallel for schedule(static)
    for (int32_t y = 0; y < static_cast<int32_t>(image.height); ++y)
        for (int32_t x = 0; x < static_cast<int32_t>(image.width); ++x)
            pixels[y * image.width + x].a = static_cast<uint8_t>(255 - pixels[y * image.width + x].r);
}

void packChannel(Image& image, const Image& texture, const uint32_t channel) {
    assert(image.width == texture.width && image.height == texture.height);

    auto& pixels    = image.pixels;
    const auto& tex = texture.pixels;

    #pragma omp parallel for schedule(static)
    for (int32_t y = 0; y < static_cast<int32_t>(image.height); ++y)
        for (int32_t x = 0; x < static_cast<int32_t>(image.width); ++x)
            pixels[y * image.width + x][channel] = tex[y * texture.width + x][channel];
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
