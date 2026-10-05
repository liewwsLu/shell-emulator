#pragma once

#include <iostream>
#include <string>

struct Config {
    std::string vfsPath;
    std::string scriptPath;
};

Config parseArguments(int argc, char* argv[]);
void printConfig(const Config& config, std::ostream& output);
