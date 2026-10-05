#include "shell.h"

#include "calendar.h"
#include "system_info.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdexcept>

void Shell::cmdLs(const std::vector<std::string>& args) {
    if (args.size() > 1) {
        throw std::runtime_error("ls: too many arguments");
    }
    std::string path = args.empty() ? "." : args[0];
    Node* node = vfs.find(path);
    if (node == nullptr) {
        throw std::runtime_error("ls: cannot access '" + path + "': no such file or directory");
    }

    if (!node->isDirectory) {
        output << node->name << "\n";
        return;
    }
    for (const auto& item : node->children) {
        output << item.first;
        if (item.second->isDirectory) {
            output << "/";
        }
        output << "\n";
    }
}

void Shell::cmdCd(const std::vector<std::string>& args) {
    if (args.size() > 1) {
        throw std::runtime_error("cd: too many arguments");
    }
    std::string path = args.empty() ? "/" : args[0];
    Node* node = vfs.find(path);
    if (node == nullptr) {
        throw std::runtime_error("cd: " + path + ": no such file or directory");
    }
    if (!node->isDirectory) {
        throw std::runtime_error("cd: " + path + ": not a directory");
    }
    vfs.setCurrent(node);
}

void Shell::cmdExit(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("exit: too many arguments");
    }
    running = false;
}

void Shell::cmdVfsInit(const std::vector<std::string>& args) {
    if (!args.empty()) {
        throw std::runtime_error("vfs-init: too many arguments");
    }
    vfs.resetToDefault();
    if (!vfsPath.empty()) {
        std::ofstream file(vfsPath, std::ios::binary | std::ios::trunc);
        if (!file) {
            throw std::runtime_error("vfs-init: cannot write " + vfsPath);
        }
        file << DEFAULT_VFS_XML;
    }
    output << "vfs-init: virtual file system was reset to default\n";
    printVfsInfo();
}

void Shell::cmdUname(const std::vector<std::string>& args) {
    bool showSystem = args.empty();
    bool showNode = false;
    bool showMachine = false;

    for (const std::string& arg : args) {
        if (arg == "-s") {
            showSystem = true;
        } else if (arg == "-n") {
            showNode = true;
        } else if (arg == "-m") {
            showMachine = true;
        } else if (arg == "-a") {
            showSystem = true;
            showNode = true;
            showMachine = true;
        } else {
            throw std::runtime_error("uname: invalid option '" + arg + "'");
        }
    }

    std::vector<std::string> parts;
    if (showSystem) {
        parts.push_back(getOsName());
    }
    if (showNode) {
        parts.push_back(getHostName());
    }
    if (showMachine) {
        parts.push_back(getMachineName());
    }
    for (size_t i = 0; i < parts.size(); i++) {
        output << (i > 0 ? " " : "") << parts[i];
    }
    output << "\n";
}

static int parseNumber(const std::string& text, int minValue, int maxValue, const std::string& what) {
    int value = 0;
    size_t used = 0;
    try {
        value = std::stoi(text, &used);
    } catch (const std::exception&) {
        used = 0;
    }
    if (used != text.size() || text.empty() || value < minValue || value > maxValue) {
        throw std::runtime_error("cal: invalid " + what + " '" + text + "'");
    }
    return value;
}

void Shell::cmdCal(const std::vector<std::string>& args) {
    int month = 0;
    int year = 0;

    if (args.empty()) {
        std::time_t now = std::time(nullptr);
        std::tm* local = std::localtime(&now);
        month = local->tm_mon + 1;
        year = local->tm_year + 1900;
    } else if (args.size() == 2) {
        month = parseNumber(args[0], 1, 12, "month");
        year = parseNumber(args[1], 1, 9999, "year");
    } else {
        throw std::runtime_error("cal: usage: cal [month year]");
    }
    output << formatMonth(month, year);
}

void Shell::cmdRev(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw std::runtime_error("rev: missing file operand");
    }
    for (const std::string& path : args) {
        Node* node = vfs.find(path);
        if (node == nullptr) {
            throw std::runtime_error("rev: " + path + ": no such file or directory");
        }
        if (node->isDirectory) {
            throw std::runtime_error("rev: " + path + ": is a directory");
        }

        std::istringstream lines(node->content);
        std::string line;
        while (std::getline(lines, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            std::reverse(line.begin(), line.end());
            output << line << "\n";
        }
    }
}

Node* Shell::findParentFor(const std::string& path, std::string& name, const std::string& command) {
    std::string directoryPath;
    splitPath(path, directoryPath, name);
    if (name.empty() || name == "." || name == "..") {
        throw std::runtime_error(command + ": invalid name '" + path + "'");
    }
    Node* directory = vfs.find(directoryPath);
    if (directory == nullptr || !directory->isDirectory) {
        throw std::runtime_error(command + ": " + directoryPath + ": no such directory");
    }
    return directory;
}

void Shell::cmdTouch(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw std::runtime_error("touch: missing file operand");
    }
    for (const std::string& path : args) {
        if (vfs.find(path) != nullptr) {
            continue;
        }
        std::string name;
        Node* directory = findParentFor(path, name, "touch");
        vfs.createFile(directory, name);
    }
}

void Shell::cmdMv(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        throw std::runtime_error("mv: expected source and destination");
    }
    Node* source = vfs.find(args[0]);
    if (source == nullptr) {
        throw std::runtime_error("mv: cannot stat '" + args[0] + "': no such file or directory");
    }
    if (source == vfs.root()) {
        throw std::runtime_error("mv: cannot move the root directory");
    }

    Node* target = vfs.find(args[1]);
    Node* newParent = nullptr;
    std::string newName;
    if (target != nullptr && target->isDirectory) {
        newParent = target;
        newName = source->name;
    } else {
        newParent = findParentFor(args[1], newName, "mv");
    }

    if (source->isDirectory && isInside(newParent, source)) {
        throw std::runtime_error("mv: cannot move a directory into itself");
    }
    auto existing = newParent->children.find(newName);
    if (existing != newParent->children.end()) {
        if (existing->second.get() == source) {
            return;
        }
        if (existing->second->isDirectory || source->isDirectory) {
            throw std::runtime_error("mv: cannot overwrite '" + newName + "'");
        }
    }
    vfs.move(source, newParent, newName);
}
