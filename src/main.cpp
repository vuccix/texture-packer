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
    Pack::unity(paths, outPath);

    std::cout << "done\n";
}
