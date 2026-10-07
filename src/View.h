#pragma once

#include "TexturePacking.h"
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

class View {
public:
    View();

    void draw();

private:
    std::array<fs::path, 3> m_paths;
    Pack::Format            m_format{};
};
