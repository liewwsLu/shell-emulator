#pragma once

#include <map>
#include <string>
#include <vector>

struct XmlElement {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string text;
    std::vector<XmlElement> children;
};

XmlElement parseXml(const std::string& text);
