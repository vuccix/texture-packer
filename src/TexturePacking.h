#pragma once

#include <Image/Image.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace Pack {

    enum class Format { Unity, ORM };
    void process(Bundle& bundle, const fs::path& outPath, Format format);

}
