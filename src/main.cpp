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

int main() {
    setupConsole();
    Shell shell(std::cout);
    shell.runInteractive(std::cin);
    return 0;
}
