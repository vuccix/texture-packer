#include "IO.h"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <cstring>

Image IO::load(const fs::path& path) {
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

void IO::save(const Image& image, const fs::path& path) {
    const std::string str = path.generic_string();
    const     int32_t w   = static_cast<int32_t>(image.width);
    const     int32_t h   = static_cast<int32_t>(image.height);
    constexpr int32_t ch  = 4;

    if (stbi_write_png(str.c_str(), w, h, ch, image.pixels.data(), (w * ch)) == 0)
        throw std::runtime_error("could not save png");
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
