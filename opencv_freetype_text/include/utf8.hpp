#pragma once

#include <cstdint>
#include <string>
#include <vector>

inline bool is_ascii_whitespace(uint32_t cp)
{
    return cp == 0x20 || cp == 0x09 || cp == 0x0D || cp == 0x0A;
}

inline bool is_cjk(uint32_t cp)
{
    return (cp >= 0x4E00 && cp <= 0x9FFF) ||
           (cp >= 0x3400 && cp <= 0x4DBF) ||
           (cp >= 0xF900 && cp <= 0xFAFF) ||
           (cp >= 0x3040 && cp <= 0x30FF) ||
           (cp >= 0x31F0 && cp <= 0x31FF);
}

inline bool is_ascii_letter(uint32_t cp)
{
    return (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z');
}

inline bool is_ascii_digit(uint32_t cp)
{
    return cp >= '0' && cp <= '9';
}

inline bool is_line_start_punctuation(uint32_t cp)
{
    switch (cp) {
    case 0xFF0C: // ，
    case 0x3002: // 。
    case 0xFF01: // ！
    case 0xFF1F: // ？
    case 0xFF1A: // ：
    case 0xFF1B: // ；
    case 0x3001: // 、
    case 0xFF09: // ）
    case 0x300D: // 】
    case 0x300F: // 』
    case 0xFF5D: // ］
    case 0x201D: // ”
    case 0x2019: // ’
        return true;
    default:
        return false;
    }
}

inline bool is_line_end_punctuation(uint32_t cp)
{
    switch (cp) {
    case 0xFF08: // （
    case 0x300A: // 《
    case 0x3008: // 〈
    case 0x300C: // 「
    case 0x300E: // 『
    case 0x201C: // “
    case 0x2018: // ‘
        return true;
    default:
        return false;
    }
}

std::vector<uint32_t> decode_utf8(const std::string& text);
