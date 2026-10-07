# Texture Packer

A very simple app for packing Metallic, Roughness and Ambient Occlusion texture maps into a single PNG file via a intuitive GUI.

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

## How to build

### Requirements

To build this project, you will need:

- A C++26 compatible compiler
- CMake (version 4.1 or higher)
- OpenGL 3.3

### Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Dependencies

- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLAD](https://glad.dav1d.de/)
- [GLFW](https://www.glfw.org/)
- [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended)
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)

---

This project was hand written in C++, no AI slop was used.
