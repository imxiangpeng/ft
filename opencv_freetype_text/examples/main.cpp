#include <opencv2/opencv.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "freetype_font.hpp"
#include "opencv_text_renderer.hpp"

namespace {

std::string find_font_path()
{
    const std::vector<std::string> candidates = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Heiti.ttc",
        "/System/Library/Fonts/STHeiti Light.ttc",
        "C:/Windows/Fonts/msyh.ttc",
        "C:/Windows/Fonts/msyhbd.ttc",
        "C:/Windows/Fonts/simhei.ttf",
        "C:/Windows/Fonts/simsun.ttc"
    };

    for (const auto& path : candidates) {
        std::ifstream in(path);
        if (in.good()) {
            return path;
        }
    }

    return "";
}

} // namespace

int main()
{
    std::string font_path = find_font_path();
    if (font_path.empty()) {
        std::cerr << "No valid font file was found. Please install a TTF/OTF/CJK font and update the font path.\n";
        return 1;
    }

    try {
        FreeTypeFont font(font_path, 32);

        std::string text =
            "这是一个中文自动换行测试文本，用于验证 OpenCV + FreeType 的中文渲染能力。\n"
            "这里是第二行，继续测试中英文混排和宽度断行。\n"
            "This is English text mixed with Chinese. It should wrap properly by width.";

        cv::Mat canvas = cv::Mat::zeros(700, 1200, CV_8UC3);
        canvas.setTo(cv::Scalar(30, 30, 30));

        draw_text(
            canvas,
            font,
            text,
            cv::Point(30, 40),
            cv::Scalar(0, 255, 255),
            1080,
            8,
            1.0f
        );

        cv::imwrite("output.png", canvas);
        std::cout << "Saved output.png using font: " << font_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
