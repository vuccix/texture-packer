#pragma once

#include <Image/Image.h>
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

namespace IO {

    Image load(const fs::path& path);
    void  save(const Image& image, const fs::path& path);

    std::pair<uint32_t, uint32_t> imgInfo(const fs::path& path);

    enum class Path { Load, Save };
    std::optional<fs::path> getPath(Path pathType);

}
