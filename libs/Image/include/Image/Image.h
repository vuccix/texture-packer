#pragma once

#include <cstdint>
#include <vector>

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
    uint32_t           width  = 0;
    uint32_t           height = 0;

public:
    bool hasTex() const;
};

struct Bundle {
    Image    metallic;
    Image    roughness;
    Image    occlusion;

    uint32_t width  = 0;
    uint32_t height = 0;

public:
    void clear();
};
