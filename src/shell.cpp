#include "shell.h"

#include "parser.h"
#include "system_info.h"

#include <fstream>
#include <stdexcept>

Shell::Shell(std::ostream& output) : output(output), running(true) {
}

void Shell::loadVfs(const std::string& path) {
    vfs.loadFromFile(path);
    vfsPath = path;
}

void Shell::printVfsInfo() const {
    int directories = 0;
    int files = 0;
    vfs.countNodes(directories, files);
    output << "[vfs] source = " << (vfsPath.empty() ? "(default)" : vfsPath)
           << ", directories: " << directories << ", files: " << files << "\n";
}

std::string Shell::prompt() const {
    return getUserName() + "@" + getHostName() + ":" + vfs.pathOf(vfs.current()) + "$ ";
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
    } else if (name == "vfs-init") {
        cmdVfsInit(rest);
    } else if (name == "uname") {
        cmdUname(rest);
    } else if (name == "cal") {
        cmdCal(rest);
    } else if (name == "rev") {
        cmdRev(rest);
    } else if (name == "touch") {
        cmdTouch(rest);
    } else if (name == "mv") {
        cmdMv(rest);
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

void Shell::runScript(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        output << "error: cannot open script: " << path << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0;
    while (running && std::getline(file, line)) {
        lineNumber++;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        output << prompt() << line << "\n";
        if (!execute(line)) {
            output << "[script] line " << lineNumber << " skipped\n";
        }
    }
}
