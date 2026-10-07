#include "TexturePacking.h"
#include <IO/IO.h>
#include <omp.h>
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

Image unity(std::array<Image, 3>& textures, const std::array<char, 3>& hasTex, const uint32_t width, const uint32_t height) {
    constexpr Pixel defaultPixel = {
        .r = 0,   // metallic
        .g = 255, // occlusion
        .b = 0,   // not used
        .a = 127, // smoothness
    };

    Image result = {
        .pixels  = std::vector(width * height, defaultPixel),
        .width   = width,
        .height  = height,
    };

    if (hasTex[0])
        ::packChannel(result, textures[0], RED);   // metallic

    if (hasTex[1])
        ::packChannel(result, textures[1], GREEN); // occlusion

    if (hasTex[2]) {
        ::roughnessToSmoothness(textures[2]);
        ::packChannel(result, textures[2], ALPHA); // smoothness
    }

    return result;
}

Image orm(const std::array<Image, 3>& textures, const std::array<char, 3>& hasTex, const uint32_t width, const uint32_t height) {
    constexpr Pixel defaultPixel = {
        .r = 255, // occlusion
        .g = 127, // roughness
        .b = 0,   // metallic
        .a = 255, // not used
    };

    Image result = {
        .pixels  = std::vector(width * height, defaultPixel),
        .width   = width,
        .height  = height,
    };

    if (hasTex[0]) ::packChannel(result, textures[0], BLUE);  // metallic
    if (hasTex[1]) ::packChannel(result, textures[1], RED);   // occlusion
    if (hasTex[2]) ::packChannel(result, textures[2], GREEN); // roughness

    return result;
}

}

void Pack::process(const std::array<fs::path, 3>& paths, const fs::path& outPath, const Format format) {
    std::array<Image, 3> textures; // 0: metallic, 1: occlusion, 2: roughness
    std::array<char,  3> hasTex = { false, false, false };

    uint32_t width = 0, height = 0;

    for (size_t i = 0; i < 3; ++i) {
        if (paths[i].empty())
            continue;

        textures[i] = IO::load(paths[i]);
        hasTex[i]   = true;

        if (width == 0 && height == 0) {
            width  = textures[i].width;
            height = textures[i].height;
        }
        else if (textures[i].width != width || textures[i].height != height)
            throw std::runtime_error("images have incompatible sizes");
    }

    if (width == 0 || height == 0)
        throw std::runtime_error("no image was loaded");

    const Image result = (format == Format::Unity)
                       ? unity(textures, hasTex, width, height)
                       : orm(textures, hasTex, width, height);

    IO::save(result, outPath);
}
