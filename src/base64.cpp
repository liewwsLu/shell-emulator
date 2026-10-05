#include "base64.h"

#include <stdexcept>

static int base64Value(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return -1;
}

std::string decodeBase64(const std::string& text) {
    std::string result;
    int buffer = 0;
    int bits = 0;

    for (char c : text) {
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            continue;
        }
        if (c == '=') {
            break;
        }
        int value = base64Value(c);
        if (value < 0) {
            throw std::runtime_error("invalid base64 data");
        }
        buffer = (buffer << 6) | value;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            result += static_cast<char>((buffer >> bits) & 0xFF);
            buffer &= (1 << bits) - 1;
        }
    }
    return result;
}
