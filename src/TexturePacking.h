#pragma once

#include <filesystem>
#include <cstdint>
#include <vector>

namespace fs = std::filesystem;

struct Pixel {
    uint8_t r =   0; // metallic
    uint8_t g = 255; // occlusion
    uint8_t b =   0; // not used
    uint8_t a = 127; // smoothness

    uint8_t  operator[](uint32_t channel) const;
    uint8_t& operator[](uint32_t channel);
};

struct Image {
    std::vector<Pixel> pixels;
    uint32_t           width;
    uint32_t           height;
};

namespace Pack {

    void unity(const std::array<fs::path, 3>& paths, const fs::path& outPath);

    void orm(const std::array<fs::path, 3>& paths, const fs::path& outPath);

}
