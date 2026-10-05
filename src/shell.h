#pragma once

#include <iostream>
#include <string>
#include <vector>

class Shell {
public:
    explicit Shell(std::ostream& output);

    std::string prompt() const;
    bool execute(const std::string& line);
    void runInteractive(std::istream& input);
    void runScript(const std::string& path);
    bool isRunning() const;

private:
    std::ostream& output;
    bool running;

    void runCommand(const std::vector<std::string>& args);
    void cmdLs(const std::vector<std::string>& args);
    void cmdCd(const std::vector<std::string>& args);
    void cmdExit(const std::vector<std::string>& args);
};
