#pragma once

#include <opencv2/opencv.hpp>

#include <string>

#include "freetype_font.hpp"
#include "text_layout.hpp"

void draw_text(
    cv::Mat& canvas,
    FreeTypeFont& font,
    const std::string& text,
    const cv::Point& origin,
    const cv::Scalar& color,
    int max_width = 0,
    int line_spacing = 4,
    float alpha = 1.0f
);

cv::Mat render_text_image(
    const std::string& text,
    FreeTypeFont& font,
    int width,
    int height,
    const cv::Scalar& background,
    const cv::Scalar& foreground,
    int max_width = 0,
    int line_spacing = 4,
    float alpha = 1.0f
);
