#include "freetype_font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstring>
#include <stdexcept>

FreeTypeFont::FreeTypeFont(const std::string& font_path, int pixel_size)
{
    if (FT_Init_FreeType(reinterpret_cast<FT_Library*>(&library_)) != 0) {
        throw std::runtime_error("FT_Init_FreeType failed");
    }

    FT_Face face = nullptr;
    if (FT_New_Face(
            reinterpret_cast<FT_Library>(library_),
            font_path.c_str(),
            0,
            &face) != 0) {
        FT_Done_FreeType(reinterpret_cast<FT_Library>(library_));
        library_ = nullptr;
        throw std::runtime_error("FT_New_Face failed: " + font_path);
    }

    face_ = face;

    if (FT_Set_Pixel_Sizes(face, 0, pixel_size) != 0) {
        FT_Done_Face(face);
        FT_Done_FreeType(reinterpret_cast<FT_Library>(library_));
        face_ = nullptr;
        library_ = nullptr;
        throw std::runtime_error("FT_Set_Pixel_Sizes failed");
    }

    ascent_ = static_cast<int>(face->size->metrics.ascender >> 6);
    descent_ = static_cast<int>(-(face->size->metrics.descender >> 6));
    line_height_ = static_cast<int>(face->size->metrics.height >> 6);
    if (line_height_ <= 0) {
        line_height_ = ascent_ + descent_;
    }
}

FreeTypeFont::~FreeTypeFont()
{
    if (face_ != nullptr) {
        FT_Done_Face(reinterpret_cast<FT_Face>(face_));
        face_ = nullptr;
    }
    if (library_ != nullptr) {
        FT_Done_FreeType(reinterpret_cast<FT_Library>(library_));
        library_ = nullptr;
    }
}

uint32_t FreeTypeFont::glyph_index(uint32_t code_point) const
{
    return FT_Get_Char_Index(reinterpret_cast<FT_Face>(face_), code_point);
}

const GlyphBitmap& FreeTypeFont::glyph(uint32_t glyph_id)
{
    auto it = cache_.find(glyph_id);
    if (it != cache_.end()) {
        return it->second;
    }

    GlyphBitmap g = load_glyph(glyph_id);
    auto result = cache_.emplace(glyph_id, std::move(g));
    return result.first->second;
}

GlyphBitmap FreeTypeFont::load_glyph(uint32_t glyph_id)
{
    FT_Face face = reinterpret_cast<FT_Face>(face_);

    if (FT_Load_Glyph(face, glyph_id, FT_LOAD_DEFAULT) != 0) {
        return GlyphBitmap{};
    }

    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) != 0) {
        return GlyphBitmap{};
    }

    GlyphBitmap bmp;
    bmp.glyph_id = glyph_id;
    bmp.width = face->glyph->bitmap.width;
    bmp.height = face->glyph->bitmap.rows;
    bmp.bearing_x = face->glyph->bitmap_left;
    bmp.bearing_y = face->glyph->bitmap_top;
    bmp.advance_x = static_cast<int>(face->glyph->advance.x >> 6);

    const size_t size = static_cast<size_t>(bmp.width * bmp.height);
    bmp.data.resize(size);

    if (size > 0) {
        const unsigned char* src = face->glyph->bitmap.buffer;
        for (int y = 0; y < bmp.height; ++y) {
            const unsigned char* row = src + y * face->glyph->bitmap.pitch;
            unsigned char* dst = bmp.data.data() + y * bmp.width;
            std::memcpy(dst, row, static_cast<size_t>(bmp.width));
        }
    }

    return bmp;
}

int FreeTypeFont::ascent() const
{
    return ascent_;
}

int FreeTypeFont::descent() const
{
    return descent_;
}

int FreeTypeFont::line_height() const
{
    return line_height_;
}

void FreeTypeFont::clear_cache()
{
    cache_.clear();
}
