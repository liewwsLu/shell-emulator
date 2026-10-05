#include "xml.h"

#include <cctype>
#include <stdexcept>

class XmlParser {
public:
    explicit XmlParser(const std::string& text) : text(text), pos(0) {
    }

    XmlElement parseDocument() {
        skipSpaces();
        if (startsWith("<?")) {
            size_t end = text.find("?>", pos);
            if (end == std::string::npos) {
                throw error("unclosed declaration");
            }
            pos = end + 2;
        }
        skipSpaces();
        XmlElement root = parseElement();
        skipSpaces();
        if (pos != text.size()) {
            throw error("unexpected content after the root element");
        }
        return root;
    }

private:
    const std::string& text;
    size_t pos;

    std::runtime_error error(const std::string& message) const {
        return std::runtime_error("xml: " + message + " at position " + std::to_string(pos));
    }

    bool startsWith(const std::string& part) const {
        return text.compare(pos, part.size(), part) == 0;
    }

    void skipSpaces() {
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) {
            pos++;
        }
    }

    void expect(char c) {
        if (pos >= text.size() || text[pos] != c) {
            throw error(std::string("expected '") + c + "'");
        }
        pos++;
    }

    std::string readName() {
        size_t start = pos;
        while (pos < text.size()) {
            char c = text[pos];
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_' && c != ':') {
                break;
            }
            pos++;
        }
        if (start == pos) {
            throw error("expected a name");
        }
        return text.substr(start, pos - start);
    }

    std::string readQuotedValue() {
        if (pos >= text.size() || (text[pos] != '"' && text[pos] != '\'')) {
            throw error("expected a quoted value");
        }
        char quote = text[pos];
        size_t end = text.find(quote, pos + 1);
        if (end == std::string::npos) {
            throw error("unclosed attribute value");
        }
        std::string value = text.substr(pos + 1, end - pos - 1);
        pos = end + 1;
        return value;
    }

    void parseAttributes(XmlElement& element) {
        while (true) {
            skipSpaces();
            if (pos >= text.size()) {
                throw error("unexpected end of file");
            }
            if (text[pos] == '>' || startsWith("/>")) {
                return;
            }
            std::string key = readName();
            skipSpaces();
            expect('=');
            skipSpaces();
            element.attributes[key] = readQuotedValue();
        }
    }

    void parseContent(XmlElement& element) {
        while (true) {
            if (pos >= text.size()) {
                throw error("unclosed element <" + element.name + ">");
            }
            if (startsWith("</")) {
                pos += 2;
                if (readName() != element.name) {
                    throw error("closing tag does not match <" + element.name + ">");
                }
                skipSpaces();
                expect('>');
                return;
            }
            if (text[pos] == '<') {
                element.children.push_back(parseElement());
            } else {
                element.text += text[pos];
                pos++;
            }
        }
    }

    XmlElement parseElement() {
        expect('<');
        XmlElement element;
        element.name = readName();
        parseAttributes(element);
        if (startsWith("/>")) {
            pos += 2;
            return element;
        }
        expect('>');
        parseContent(element);
        return element;
    }
};

XmlElement parseXml(const std::string& text) {
    XmlParser parser(text);
    return parser.parseDocument();
}
