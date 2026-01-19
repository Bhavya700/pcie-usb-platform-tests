#pragma once

#include <string>

namespace utils {

class FsHelper {
public:
    // Read the entire content of a sysfs/debugfs node
    static std::string readNode(const std::string& path, std::string* errorMsg = nullptr);

    // Write a string value to a sysfs/debugfs node
    static bool writeNode(const std::string& path, const std::string& value, std::string& errorMsg);

    // Write an integer value to a sysfs/debugfs node
    static bool writeNode(const std::string& path, int value, std::string& errorMsg);

    // Read an integer value from a sysfs/debugfs node
    static bool readNodeToInt(const std::string& path, int& outValue, std::string& errorMsg);
};

} // namespace utils
