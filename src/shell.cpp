#include "shell.h"

#include "parser.h"
#include "system_info.h"

#include <stdexcept>

Shell::Shell(std::ostream& output) : output(output), running(true) {
}

std::string Shell::prompt() const {
    return getUserName() + "@" + getHostName() + ":~$ ";
}

bool Shell::isRunning() const {
    return running;
}

bool Shell::execute(const std::string& line) {
    try {
        std::vector<std::string> args = parseLine(line);
        if (!args.empty()) {
            runCommand(args);
        }
        return true;
    } catch (const std::exception& error) {
        output << "error: " << error.what() << "\n";
        return false;
    }
}

void Shell::runCommand(const std::vector<std::string>& args) {
    const std::string& name = args[0];
    std::vector<std::string> rest(args.begin() + 1, args.end());

    if (name == "ls") {
        cmdLs(rest);
    } else if (name == "cd") {
        cmdCd(rest);
    } else if (name == "exit") {
        cmdExit(rest);
    } else {
        throw std::runtime_error(name + ": command not found");
    }
}

void Shell::runInteractive(std::istream& input) {
    std::string line;
    while (running) {
        output << prompt();
        if (!std::getline(input, line)) {
            output << "\n";
            break;
        }
        execute(line);
    }
}
