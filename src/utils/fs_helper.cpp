#include "fs_helper.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace utils {

std::string FsHelper::readNode(const std::string& path, std::string* errorMsg) {
    std::ifstream file(path);
    if (!file.is_open()) {
        if (errorMsg) *errorMsg = "Failed to open node for reading: " + path;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FsHelper::writeNode(const std::string& path, const std::string& value, std::string& errorMsg) {
    std::ofstream file(path);
    if (!file.is_open()) {
        errorMsg = "Failed to open node for writing: " + path;
        return false;
    }
    file << value;
    if (file.fail()) {
        errorMsg = "Failed to write value '" + value + "' to node: " + path;
        return false;
    }
    return true;
}

bool FsHelper::writeNode(const std::string& path, int value, std::string& errorMsg) {
    return writeNode(path, std::to_string(value), errorMsg);
}

bool FsHelper::readNodeToInt(const std::string& path, int& outValue, std::string& errorMsg) {
    std::string content = readNode(path, &errorMsg);
    if (content.empty()) {
        return false;
    }
    try {
        outValue = std::stoi(content);
        return true;
    } catch (const std::exception& e) {
        errorMsg = "Failed to parse integer from node content: " + content + " (" + e.what() + ")";
        return false;
    }
}

} // namespace utils
