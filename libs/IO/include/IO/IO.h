#pragma once

#include <filesystem>
#include <optional>
#include <cstdint>
#include <vector>

namespace fs = std::filesystem;

struct Pixel { // Unity      | ORM
    uint8_t r; // metallic   | occlusion
    uint8_t g; // occlusion  | roughness
    uint8_t b; // not used   | metallic
    uint8_t a; // smoothness | not used

    uint8_t  operator[](uint32_t channel) const;
    uint8_t& operator[](uint32_t channel);
};

struct Image {
    std::vector<Pixel> pixels;
    uint32_t           width;
    uint32_t           height;
};

namespace IO {

    Image load(const fs::path& path);
    void  save(const Image& image, const fs::path& path);

    enum class Path { Load, Save };
    std::optional<fs::path> getPath(Path pathType);

}
