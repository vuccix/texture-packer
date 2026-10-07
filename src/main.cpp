#include "TexturePacking.h"
#include <iostream>

std::string full = R"(...)";

std::array<fs::path, 3> paths = {
    full + R"(...)", // metallic
    full + R"(...)", // occlusion
    full + R"(...)", // roughness
};

fs::path outPath = full + R"(result.png)";

int main() {
    Pack::process(paths, outPath, Pack::Format::ORM);

    std::cout << "done\n";
}
