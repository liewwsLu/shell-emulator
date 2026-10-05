#pragma once

#include <map>
#include <memory>
#include <string>

struct Node {
    std::string name;
    bool isDirectory = false;
    std::string content;
    Node* parent = nullptr;
    std::map<std::string, std::unique_ptr<Node>> children;
};

extern const std::string DEFAULT_VFS_XML;

void splitPath(const std::string& path, std::string& directory, std::string& name);
bool isInside(const Node* node, const Node* ancestor);

class Vfs {
public:
    Vfs();

    void loadFromXml(const std::string& xmlText);
    void loadFromFile(const std::string& path);
    void resetToDefault();

    Node* root() const;
    Node* current() const;
    void setCurrent(Node* node);
    Node* find(const std::string& path) const;
    std::string pathOf(const Node* node) const;
    void createFile(Node* directory, const std::string& name);
    void move(Node* node, Node* newParent, const std::string& newName);
    void countNodes(int& directories, int& files) const;

private:
    std::unique_ptr<Node> rootNode;
    Node* currentNode;
};
