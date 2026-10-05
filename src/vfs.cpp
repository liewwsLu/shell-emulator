#include "vfs.h"

#include "base64.h"
#include "xml.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

const std::string DEFAULT_VFS_XML =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<vfs>\n"
    "  <dir name=\"home\"/>\n"
    "  <file name=\"readme.txt\">RGVmYXVsdCB2aXJ0dWFsIGZpbGUgc3lzdGVtCg==</file>\n"
    "</vfs>\n";

static void addChildren(Node& node, const XmlElement& element);

static std::string nameOf(const XmlElement& element) {
    auto it = element.attributes.find("name");
    if (it == element.attributes.end() || it->second.empty()) {
        throw std::runtime_error("vfs: element <" + element.name + "> has no name");
    }
    const std::string& name = it->second;
    if (name == "." || name == ".." || name.find('/') != std::string::npos) {
        throw std::runtime_error("vfs: invalid name '" + name + "'");
    }
    return name;
}

static std::unique_ptr<Node> buildNode(const XmlElement& element, Node* parent) {
    if (element.name != "dir" && element.name != "file") {
        throw std::runtime_error("vfs: unknown element <" + element.name + ">");
    }

    std::unique_ptr<Node> node = std::make_unique<Node>();
    node->name = nameOf(element);
    node->parent = parent;
    node->isDirectory = element.name == "dir";

    if (node->isDirectory) {
        addChildren(*node, element);
    } else {
        if (!element.children.empty()) {
            throw std::runtime_error("vfs: file '" + node->name + "' cannot contain elements");
        }
        try {
            node->content = decodeBase64(element.text);
        } catch (const std::runtime_error& error) {
            throw std::runtime_error("vfs: file '" + node->name + "': " + error.what());
        }
    }
    return node;
}

static void addChildren(Node& node, const XmlElement& element) {
    for (const XmlElement& child : element.children) {
        std::unique_ptr<Node> childNode = buildNode(child, &node);
        if (node.children.count(childNode->name) > 0) {
            throw std::runtime_error("vfs: duplicate name '" + childNode->name + "'");
        }
        node.children[childNode->name] = std::move(childNode);
    }
}

Vfs::Vfs() : currentNode(nullptr) {
    resetToDefault();
}

void Vfs::loadFromXml(const std::string& xmlText) {
    XmlElement document = parseXml(xmlText);
    if (document.name != "vfs") {
        throw std::runtime_error("vfs: root element must be <vfs>");
    }

    std::unique_ptr<Node> newRoot = std::make_unique<Node>();
    newRoot->isDirectory = true;
    addChildren(*newRoot, document);

    rootNode = std::move(newRoot);
    currentNode = rootNode.get();
}

void Vfs::loadFromFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("vfs: cannot open file " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    loadFromXml(buffer.str());
}

void Vfs::resetToDefault() {
    loadFromXml(DEFAULT_VFS_XML);
}

Node* Vfs::root() const {
    return rootNode.get();
}

Node* Vfs::current() const {
    return currentNode;
}

void Vfs::setCurrent(Node* node) {
    currentNode = node;
}

Node* Vfs::find(const std::string& path) const {
    Node* node = currentNode;
    if (!path.empty() && path[0] == '/') {
        node = rootNode.get();
    }

    std::stringstream parts(path);
    std::string part;
    while (std::getline(parts, part, '/')) {
        if (part.empty() || part == ".") {
            continue;
        }
        if (part == "..") {
            if (node->parent != nullptr) {
                node = node->parent;
            }
            continue;
        }
        if (!node->isDirectory) {
            return nullptr;
        }
        auto it = node->children.find(part);
        if (it == node->children.end()) {
            return nullptr;
        }
        node = it->second.get();
    }
    return node;
}

std::string Vfs::pathOf(const Node* node) const {
    if (node->parent == nullptr) {
        return "/";
    }
    std::string path;
    for (const Node* cur = node; cur->parent != nullptr; cur = cur->parent) {
        path = "/" + cur->name + path;
    }
    return path;
}

void Vfs::createFile(Node* directory, const std::string& name) {
    std::unique_ptr<Node> file = std::make_unique<Node>();
    file->name = name;
    file->parent = directory;
    directory->children[name] = std::move(file);
}

void Vfs::move(Node* node, Node* newParent, const std::string& newName) {
    Node* oldParent = node->parent;
    std::unique_ptr<Node> owned = std::move(oldParent->children[node->name]);
    oldParent->children.erase(node->name);

    node->name = newName;
    node->parent = newParent;
    newParent->children[newName] = std::move(owned);
}

void splitPath(const std::string& path, std::string& directory, std::string& name) {
    size_t slash = path.find_last_of('/');
    if (slash == std::string::npos) {
        directory = ".";
        name = path;
    } else if (slash == 0) {
        directory = "/";
        name = path.substr(1);
    } else {
        directory = path.substr(0, slash);
        name = path.substr(slash + 1);
    }
}

bool isInside(const Node* node, const Node* ancestor) {
    for (const Node* cur = node; cur != nullptr; cur = cur->parent) {
        if (cur == ancestor) {
            return true;
        }
    }
    return false;
}

static void countRecursive(const Node* node, int& directories, int& files) {
    for (const auto& item : node->children) {
        if (item.second->isDirectory) {
            directories++;
            countRecursive(item.second.get(), directories, files);
        } else {
            files++;
        }
    }
}

void Vfs::countNodes(int& directories, int& files) const {
    directories = 0;
    files = 0;
    countRecursive(rootNode.get(), directories, files);
}
