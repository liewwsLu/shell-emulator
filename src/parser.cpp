#include "parser.h"

#include <stdexcept>

static bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

std::vector<std::string> parseLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string current;
    char quote = 0;
    bool started = false;

    for (char c : line) {
        if (quote != 0) {
            if (c == quote) {
                quote = 0;
            } else {
                current += c;
            }
        } else if (c == '"' || c == '\'') {
            quote = c;
            started = true;
        } else if (isSpace(c)) {
            if (started) {
                tokens.push_back(current);
                current.clear();
                started = false;
            }
        } else {
            current += c;
            started = true;
        }
    }

    if (quote != 0) {
        throw std::runtime_error("unclosed quote");
    }
    if (started) {
        tokens.push_back(current);
    }
    return tokens;
}
