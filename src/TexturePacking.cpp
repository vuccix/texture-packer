#include "TexturePacking.h"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <omp.h>
#include <cstring>
#include <vector>
#include <mdspan>

namespace {

Image load(const fs::path& path) {
    const std::string& str = path.generic_string();
    int32_t w, h, _;

    uint8_t* data = stbi_load(str.c_str(), &w, &h, &_, 4);

    if (!data)            throw std::runtime_error("could not load");
    if (w == 0 || h == 0) throw std::runtime_error("incorrect image size");

    const size_t pixelCount = static_cast<size_t>(w) * static_cast<size_t>(h);

    Image result;
    result.width  = static_cast<uint32_t>(w);
    result.height = static_cast<uint32_t>(h);
    result.pixels.resize(pixelCount);

    std::memcpy(
        result.pixels.data(),
        data,
        pixelCount * sizeof(Pixel)
    );

    stbi_image_free(data);
    return result;
}

void save(const Image& image, const fs::path& path) {
    const std::string str = path.generic_string();
    const     int32_t w   = static_cast<int32_t>(image.width);
    const     int32_t h   = static_cast<int32_t>(image.height);
    constexpr int32_t ch  = 4;

    if (stbi_write_png(str.c_str(), w, h, ch, image.pixels.data(), (w * ch)) == 0)
        throw std::runtime_error("could not save png");
}

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

}

void Pack::unity(const std::array<fs::path, 3>& paths, const fs::path& outPath) {
    std::array<Image, 3> textures; // 0: metallic, 1: occlusion, 2: roughness
    std::array<char,  3> hasTex{}; // 0: metallic, 1: occlusion, 2: roughness

    uint32_t width = 0, height = 0;

    for (size_t i = 0; i < 3; ++i) {
        if (!paths[i].empty()) {
            textures[i] = ::load(paths[i]);
            hasTex[i]   = true;
            width       = textures[i].width;
            height      = textures[i].height;
        }
    }

    Image result = {
        .pixels  = std::vector<Pixel>(width * height),
        .width   = width,
        .height  = height,
    };

    if (hasTex[0])
        ::packChannel(result, textures[0], 0); // metallic

    if (hasTex[1])
        ::packChannel(result, textures[1], 1); // occlusion

    if (hasTex[2]) {
        ::roughnessToSmoothness(textures[2]);
        ::packChannel(result, textures[2], 3); // smoothness
    }

    ::save(result, outPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t& Pixel::operator[](const uint32_t channel) {
    switch (channel) {
        case 0:  return r;
        case 1:  return g;
        case 2:  return b;
        default: return a;
    }
}

uint8_t Pixel::operator[](const uint32_t channel) const {
    return const_cast<Pixel&>(*this)[channel];
}
