#include "opencv_text_renderer.hpp"

#include <algorithm>
#include <iostream>

namespace {

void blend_glyph(
    cv::Mat& canvas,
    const GlyphBitmap& glyph,
    int x,
    int y,
    const cv::Scalar& color,
    float alpha
)
{
    const int start_x = x + glyph.bearing_x;
    const int start_y = y - glyph.bearing_y;

    for (int row = 0; row < glyph.height; ++row) {
        const int py = start_y + row;
        if (py < 0 || py >= canvas.rows) {
            continue;
        }

        for (int col = 0; col < glyph.width; ++col) {
            const int px = start_x + col;
            if (px < 0 || px >= canvas.cols) {
                continue;
            }

            const unsigned char coverage = glyph.data[static_cast<size_t>(row * glyph.width + col)];
            if (coverage == 0) {
                continue;
            }

            const float a = alpha * (static_cast<float>(coverage) / 255.0f);
            cv::Vec3b& dst = canvas.at<cv::Vec3b>(py, px);
            for (int c = 0; c < 3; ++c) {
                float src = static_cast<float>(color[c]);
                float old = static_cast<float>(dst[c]);
                dst[c] = cv::saturate_cast<uchar>(old * (1.0f - a) + src * a);
            }
        }
    }
}

} // namespace

void draw_text(
    cv::Mat& canvas,
    FreeTypeFont& font,
    const std::string& text,
    const cv::Point& origin,
    const cv::Scalar& color,
    int max_width,
    int line_spacing,
    float alpha
)
{
    LayoutOptions options;
    options.max_width = max_width;
    options.line_spacing = line_spacing;

    TextLayout layout = layout_text(font, text, options);

    for (size_t row = 0; row < layout.lines.size(); ++row) {
        const auto& line = layout.lines[row];
        int baseline_y = origin.y + font.ascent() + static_cast<int>(row) * (font.line_height() + line_spacing);

        for (const auto& item : line.glyphs) {
            const GlyphBitmap& glyph = font.glyph(item.glyph_id);
            const int px = origin.x + static_cast<int>(item.x);
            blend_glyph(canvas, glyph, px, baseline_y, color, alpha);
        }
    }
}

cv::Mat render_text_image(
    const std::string& text,
    FreeTypeFont& font,
    int width,
    int height,
    const cv::Scalar& background,
    const cv::Scalar& foreground,
    int max_width,
    int line_spacing,
    float alpha
)
{
    cv::Mat canvas(height, width, CV_8UC3, background);
    draw_text(canvas, font, text, cv::Point(20, 30), foreground, max_width, line_spacing, alpha);
    return canvas;
}
