# Texture Packer

[![CMake CI](https://github.com/vuccix/texture-packer/actions/workflows/ci.yml/badge.svg)](https://github.com/vuccix/texture-packer/actions/workflows/ci.yml)

A very simple tool for packing Metallic, Roughness and Ambient Occlusion texture maps into a single PNG file via a intuitive GUI. Useful for Unity's URP Lit material shaders or other engines which use ORM texture packing.

## Supported Packing Formats

### Unity URP Lit / Unlit / Complex Lit Materials

| Channel | Texture    |
|---------|------------|
| Red     | Metallic   |
| Green   | Occlusion  |
| Blue    | not used   |
| Alpha   | Smoothness |

> Inserted Roughness texture will be automatically converted to Smoothness and correctly packed.

### ORM (Occlusion, Roughness, Metallic)

| Channel | Texture    |
|---------|------------|
| Red     | Occlusion  |
| Green   | Roughness  |
| Blue    | Metallic   |
| Alpha   | not used   |

## How to use

At least one texture map must be provided before being able to create the packed output. Unused maps will be filled with default values which **should not** cause visual artefacts in your games.

Provided image files must be PNGs, JPEGs, TGAs or GIFs. You may provide different file formats for different texture maps. All texture maps **must have the same dimensions**.

```cpp
Pixel default_Unity_Pixel = {
    .r = 0,   // metallic
    .g = 255, // occlusion
    .b = 0,   // not used
    .a = 127, // smoothness
};

Pixel default_ORM_Pixel = {
    .r = 255, // occlusion
    .g = 127, // roughness
    .b = 0,   // metallic
    .a = 255, // not used
};
```

## How to build

### Requirements

To build this project, you will need:

- A C++20 compatible compiler (GCC/MinGW, MSVC, Clang)
- CMake (version 3.20 or higher)
- OpenGL 3.3


- GTK 3, Wayland and X11 development files **if on Linux**
- OpenMP runtime **if on macOS**

### Building

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Dependencies

- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLAD](https://glad.dav1d.de/)
- [GLFW](https://www.glfw.org/)
- [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended)
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

---

This project was handwritten by me in C++, no AI slop was used.
