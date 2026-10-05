#pragma once

#include "vfs.h"

#include <iostream>
#include <string>
#include <vector>

class Shell {
public:
    explicit Shell(std::ostream& output);

    void loadVfs(const std::string& path);
    void printVfsInfo() const;
    std::string prompt() const;
    bool execute(const std::string& line);
    void runInteractive(std::istream& input);
    void runScript(const std::string& path);
    bool isRunning() const;

private:
    std::ostream& output;
    bool running;
    Vfs vfs;
    std::string vfsPath;

    void runCommand(const std::vector<std::string>& args);
    void cmdLs(const std::vector<std::string>& args);
    void cmdCd(const std::vector<std::string>& args);
    void cmdExit(const std::vector<std::string>& args);
    void cmdVfsInit(const std::vector<std::string>& args);
    void cmdUname(const std::vector<std::string>& args);
    void cmdCal(const std::vector<std::string>& args);
    void cmdRev(const std::vector<std::string>& args);
    void cmdTouch(const std::vector<std::string>& args);
    void cmdMv(const std::vector<std::string>& args);
    Node* findParentFor(const std::string& path, std::string& name, const std::string& command);
};
