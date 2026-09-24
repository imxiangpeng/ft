#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct GlyphBitmap
{
    uint32_t glyph_id = 0;

    int width = 0;
    int height = 0;
    int bearing_x = 0;
    int bearing_y = 0;
    int advance_x = 0;

    std::vector<unsigned char> data;
};

class FreeTypeFont
{
public:
    FreeTypeFont(const std::string& font_path, int pixel_size);
    ~FreeTypeFont();

    FreeTypeFont(const FreeTypeFont&) = delete;
    FreeTypeFont& operator=(const FreeTypeFont&) = delete;

    uint32_t glyph_index(uint32_t code_point) const;
    const GlyphBitmap& glyph(uint32_t glyph_id);

    int ascent() const;
    int descent() const;
    int line_height() const;

    void clear_cache();

private:
    GlyphBitmap load_glyph(uint32_t glyph_id);

private:
    void* library_ = nullptr;
    void* face_ = nullptr;

    int ascent_ = 0;
    int descent_ = 0;
    int line_height_ = 0;

    std::unordered_map<uint32_t, GlyphBitmap> cache_;
};
