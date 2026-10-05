#include "system_info.h"

#include <cstdlib>

#ifndef _WIN32
#include <unistd.h>
#endif

static std::string readEnv(const char* name, const std::string& fallback) {
    const char* value = std::getenv(name);
    if (value == nullptr || value[0] == '\0') {
        return fallback;
    }
    return value;
}

std::string getUserName() {
#ifdef _WIN32
    return readEnv("USERNAME", "user");
#else
    return readEnv("USER", "user");
#endif
}

std::string getHostName() {
#ifdef _WIN32
    return readEnv("COMPUTERNAME", "localhost");
#else
    char buffer[256];
    if (gethostname(buffer, sizeof(buffer)) == 0) {
        return buffer;
    }
    return "localhost";
#endif
}

std::string getOsName() {
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "Darwin";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
}

std::string getMachineName() {
#if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "aarch64";
#elif defined(__i386__) || defined(_M_IX86)
    return "i686";
#else
    return "unknown";
#endif
}
