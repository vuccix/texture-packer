#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace Pack {

    enum class Format { Unity, ORM };

    void process(const std::array<fs::path, 3>& paths, const fs::path& outPath, Format format);

}
