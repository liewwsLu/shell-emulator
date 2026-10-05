#include "config.h"
#include "shell.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(GetACP());
    SetConsoleCP(GetACP());
#endif
}

int main(int argc, char* argv[]) {
    setupConsole();

    Config config;
    try {
        config = parseArguments(argc, argv);
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << "\n";
        std::cerr << "usage: emulator [--vfs <path>] [--script <path>]\n";
        return 1;
    }
    printConfig(config, std::cout);

    Shell shell(std::cout);
    if (!config.scriptPath.empty()) {
        shell.runScript(config.scriptPath);
    }
    shell.runInteractive(std::cin);
    return 0;
}
