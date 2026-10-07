#include <IO/IO.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <nfd.hpp>
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

std::pair<uint32_t, uint32_t> IO::imgInfo(const fs::path& path) {
    int32_t w, h, _;
    if (!stbi_info(path.generic_string().c_str(), &w, &h, &_))
        throw std::runtime_error("could not load");

    return std::make_pair(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
}

std::optional<fs::path> IO::getPath(const Path pathType) {
    NFD::Guard      nfdGuard;
    NFD::UniquePath outPath;

    // filters for dialog
    constexpr std::array filterItem = {
        nfdfilteritem_t{ "PNG",  "png"           },
        nfdfilteritem_t{ "JPEG", "jpg,jpeg,jfif" },
        nfdfilteritem_t{ "TGA",  "tga"           },
        nfdfilteritem_t{ "GIF",  "gif"           },
    };

    // show dialog
    const nfdresult_t result = (pathType == Path::Load)
                             ? NFD::OpenDialog(outPath, filterItem.data(), filterItem.size())
                             : NFD::SaveDialog(outPath, filterItem.data(), filterItem.size());

    if (result == NFD_OKAY)
        return outPath.get();

    return std::nullopt;
}
