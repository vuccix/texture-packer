#pragma once

#include "TexturePacking.h"
#include <Image/Image.h>
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

class View {
public:
    View();

    void draw();

private:
    std::array<fs::path, 3> m_paths;
    Bundle                  m_bundle;
    Pack::Format            m_format{};
};
