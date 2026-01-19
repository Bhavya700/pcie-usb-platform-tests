#pragma once

#include <string>
#include <vector>
#include "usb_device_info.h"

namespace usb {

class UsbManager {
public:
    UsbManager();
    
    // Enumerates all USB devices in the sysfs root
    std::vector<UsbDeviceInfo> enumerateDevices();

private:
    std::string sysfsRoot;
    
    // Helper to read specific attribute
    std::string readAttribute(const std::string& devicePath, const std::string& attrName);
};

} // namespace usb
