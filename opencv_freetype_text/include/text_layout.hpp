#pragma once

#include <string>
#include <vector>

#include "freetype_font.hpp"

struct LayoutGlyph
{
    uint32_t code_point = 0;
    uint32_t glyph_id = 0;

    float x = 0.0f;
    float advance_x = 0.0f;
    bool whitespace = false;
};

struct LayoutLine
{
    std::vector<LayoutGlyph> glyphs;
    float width = 0.0f;
};

struct TextLayout
{
    std::vector<LayoutLine> lines;
    float width = 0.0f;
    float height = 0.0f;
};

struct LayoutOptions
{
    int max_width = 0;
    int line_spacing = 4;
    bool break_chinese = true;
    bool break_english_by_word = true;
    bool avoid_line_start_punctuation = true;
};

TextLayout layout_text(
    FreeTypeFont& font,
    const std::string& text,
    const LayoutOptions& options = LayoutOptions{}
);
