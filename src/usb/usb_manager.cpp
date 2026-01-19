#include "usb_manager.h"
#include "../utils/fs_helper.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace usb {

namespace fs = std::filesystem;

UsbManager::UsbManager() {
    const char* envPath = std::getenv("PLATFORM_TEST_USB_SYSFS_ROOT");
    if (envPath) {
        sysfsRoot = envPath;
    } else {
        sysfsRoot = "/sys/bus/usb/devices";
    }
}

std::vector<UsbDeviceInfo> UsbManager::enumerateDevices() {
    std::vector<UsbDeviceInfo> devices;
    
    if (!fs::exists(sysfsRoot)) {
        std::cerr << "USB sysfs root not found: " << sysfsRoot << std::endl;
        return devices;
    }

    for (const auto& entry : fs::directory_iterator(sysfsRoot)) {
        if (!entry.is_directory() && !entry.is_symlink()) {
            continue;
        }

        std::string path = entry.path().string();
        std::string vid = readAttribute(path, "idVendor");
        std::string pid = readAttribute(path, "idProduct");
        
        // Only include devices that have VID/PID (filters out root hubs or interfaces sometimes)
        if (!vid.empty() && !pid.empty()) {
            UsbDeviceInfo info;
            info.path = path;
            info.vendorId = vid;
            info.productId = pid;
            info.deviceClass = readAttribute(path, "bDeviceClass");
            info.deviceSubClass = readAttribute(path, "bDeviceSubClass");
            info.deviceProtocol = readAttribute(path, "bDeviceProtocol");
            info.speed = readAttribute(path, "speed");
            devices.push_back(info);
        }
    }
    
    return devices;
}

std::string UsbManager::readAttribute(const std::string& devicePath, const std::string& attrName) {
    std::string fullPath = devicePath + "/" + attrName;
    std::string content = utils::FsHelper::readNode(fullPath);
    
    // Trim newline
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    return content;
}

} // namespace usb
