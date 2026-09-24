# opencv-freetype-text

A minimal C++ project that demonstrates how to render Chinese text with FreeType and draw it into an OpenCV `cv::Mat` with automatic line wrapping.

Features:
- OpenCV + FreeType integration
- UTF-8 decoding
- Chinese and English text rendering
- Automatic width-based wrapping
- `\n` explicit line breaks
- Glyph cache for repeated rendering
- Easy CMake build

## Requirements

Ubuntu / Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libopencv-dev libfreetype6-dev
```

Windows (MSYS2 or vcpkg):
- Install `cmake`, `opencv`, `freetype`
- Configure toolchain accordingly

## Build

```bash
cd opencv_freetype_text
cmake -S . -B build
cmake --build build -j
```

## Run

```bash
./build/opencv_freetype_text
```

The example writes `output.png` in the current working directory.

## Notes

This project does not yet implement full HarfBuzz shaping. It uses FreeType glyph metrics directly and supports Chinese automatic wrapping by measuring the rendered glyph advance width.

If you need more advanced shaping for Arabic, Indic scripts, ligatures, or complex layout, the next step is to add HarfBuzz on top of FreeType.
