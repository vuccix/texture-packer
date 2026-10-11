#include <Image/Image.h>

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Image::hasTex() const {
    return width > 0 && height > 0 && !pixels.empty();
}

void Bundle::clear() {
    width = height = 0;

    metallic.pixels.clear();
    occlusion.pixels.clear();
    roughness.pixels.clear();
}
