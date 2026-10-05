#include "config.h"

#include <stdexcept>

Config parseArguments(int argc, char* argv[]) {
    Config config;
    for (int i = 1; i < argc; i++) {
        std::string option = argv[i];
        if (option != "--vfs" && option != "--script") {
            throw std::runtime_error("unknown option: " + option);
        }
        if (i + 1 >= argc) {
            throw std::runtime_error("missing value for option " + option);
        }
        i++;
        if (option == "--vfs") {
            config.vfsPath = argv[i];
        } else {
            config.scriptPath = argv[i];
        }
    }
    return config;
}

static std::string valueOrNotSet(const std::string& value) {
    if (value.empty()) {
        return "(not set)";
    }
    return value;
}

void printConfig(const Config& config, std::ostream& output) {
    output << "[config] vfs path    = " << valueOrNotSet(config.vfsPath) << "\n";
    output << "[config] script path = " << valueOrNotSet(config.scriptPath) << "\n";
}
