#include "text_layout.hpp"

#include "utf8.hpp"

#include <algorithm>
#include <vector>

TextLayout layout_text(
    FreeTypeFont& font,
    const std::string& text,
    const LayoutOptions& options
)
{
    std::vector<uint32_t> cps = decode_utf8(text);
    TextLayout layout;
    layout.lines.emplace_back();

    float current_x = 0.0f;
    uint32_t previous_codepoint = 0;

    for (uint32_t cp : cps) {
        if (cp == 0x0D) {
            continue;
        }

        if (cp == 0x0A) {
            auto& line = layout.lines.back();
            line.width = current_x;
            layout.lines.emplace_back();
            current_x = 0.0f;
            previous_codepoint = cp;
            continue;
        }

        uint32_t glyph_id = font.glyph_index(cp);
        if (glyph_id == 0) {
            glyph_id = font.glyph_index(0xFFFD);
        }

        const GlyphBitmap& glyph = font.glyph(glyph_id);
        float advance = static_cast<float>(glyph.advance_x);

        bool should_break = false;
        if (options.max_width > 0 && current_x > 0.0f && current_x + advance > static_cast<float>(options.max_width)) {
            if (options.break_chinese && (is_cjk(cp) || is_cjk(previous_codepoint) || is_line_start_punctuation(cp))) {
                should_break = true;
            } else if (is_ascii_whitespace(previous_codepoint)) {
                should_break = true;
            } else if (cp == ' ' || cp == '\t') {
                should_break = true;
            }
        }

        if (should_break) {
            LayoutLine new_line;
            new_line.width = 0.0f;
            layout.lines.push_back(new_line);
            current_x = 0.0f;
        }

        LayoutGlyph item;
        item.code_point = cp;
        item.glyph_id = glyph_id;
        item.x = current_x;
        item.advance_x = advance;
        item.whitespace = is_ascii_whitespace(cp);

        layout.lines.back().glyphs.push_back(item);
        layout.lines.back().width = std::max(layout.lines.back().width, current_x + advance);
        current_x += advance;
        previous_codepoint = cp;
    }

    if (!layout.lines.empty()) {
        layout.lines.back().width = std::max(layout.lines.back().width, current_x);
    }

    for (const auto& line : layout.lines) {
        layout.width = std::max(layout.width, line.width);
    }

    layout.height = static_cast<float>(layout.lines.size()) * static_cast<float>(font.line_height() + options.line_spacing);

    return layout;
}
