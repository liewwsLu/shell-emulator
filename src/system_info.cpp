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
