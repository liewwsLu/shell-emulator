#include "shell.h"

#include <stdexcept>

static void printStub(std::ostream& output, const std::string& name, const std::vector<std::string>& args) {
    output << name << ": stub called";
    if (args.empty()) {
        output << " without arguments";
    }
    for (const std::string& arg : args) {
        output << " [" << arg << "]";
    }
    output << "\n";
}

void Shell::cmdLs(const std::vector<std::string>& args) {
    printStub(output, "ls", args);
}

void Shell::cmdCd(const std::vector<std::string>& args) {
    if (args.size() > 1) {
        throw std::runtime_error("cd: too many arguments");
    }
    printStub(output, "cd", args);
}

void Shell::cmdExit(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("exit: too many arguments");
    }
    running = false;
}
