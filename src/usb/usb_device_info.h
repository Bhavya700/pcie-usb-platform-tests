#pragma once

#include <string>

namespace usb {

struct UsbDeviceInfo {
    std::string path;
    std::string vendorId;
    std::string productId;
    std::string deviceClass;
    std::string deviceSubClass;
    std::string deviceProtocol;
    std::string speed;
};

} // namespace usb
