#include "utf8.hpp"

#include <stdexcept>
#include <vector>

std::vector<uint32_t> decode_utf8(const std::string& text)
{
    std::vector<uint32_t> result;
    result.reserve(text.size());

    size_t i = 0;
    while (i < text.size()) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        uint32_t cp = 0;

        if ((c & 0x80U) == 0U) {
            cp = c;
            ++i;
        } else if ((c & 0xE0U) == 0xC0U) {
            if (i + 1 >= text.size()) {
                cp = 0xFFFD;
                ++i;
            } else {
                cp = ((static_cast<uint32_t>(c & 0x1FU) << 6U) |
                      static_cast<uint32_t>(text[i + 1] & 0x3FU));
                i += 2;
            }
        } else if ((c & 0xF0U) == 0xE0U) {
            if (i + 2 >= text.size()) {
                cp = 0xFFFD;
                i = text.size();
            } else {
                cp = ((static_cast<uint32_t>(c & 0x0FU) << 12U) |
                      (static_cast<uint32_t>(static_cast<unsigned char>(text[i + 1]) & 0x3FU) << 6U) |
                      static_cast<uint32_t>(static_cast<unsigned char>(text[i + 2]) & 0x3FU));
                i += 3;
            }
        } else if ((c & 0xF8U) == 0xF0U) {
            if (i + 3 >= text.size()) {
                cp = 0xFFFD;
                i = text.size();
            } else {
                cp = ((static_cast<uint32_t>(c & 0x07U) << 18U) |
                      (static_cast<uint32_t>(static_cast<unsigned char>(text[i + 1]) & 0x3FU) << 12U) |
                      (static_cast<uint32_t>(static_cast<unsigned char>(text[i + 2]) & 0x3FU) << 6U) |
                      static_cast<uint32_t>(static_cast<unsigned char>(text[i + 3]) & 0x3FU));
                i += 4;
            }
        } else {
            cp = 0xFFFD;
            ++i;
        }

        if (cp == 0) {
            continue;
        }

        result.push_back(cp);
    }

    return result;
}
