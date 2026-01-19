#include "usb_test.h"
#include <iostream>

TEST_F(UsbSysfsTest, EnumerateDevices) {
    auto devices = usbManager.enumerateDevices();
    
    // We expect at least some devices or empty if in an isolated environment
    // But this test mainly ensures the code runs without crashing
    std::cout << "Found " << devices.size() << " USB devices." << std::endl;
    
    for (const auto& dev : devices) {
        std::cout << "Device: " << dev.path << "\n"
                  << "  VID: " << dev.vendorId << "\n"
                  << "  PID: " << dev.productId << "\n"
                  << "  Speed: " << dev.speed << "\n" 
                  << std::endl;
                  
        EXPECT_FALSE(dev.vendorId.empty()) << "Vendor ID should not be empty";
        EXPECT_FALSE(dev.productId.empty()) << "Product ID should not be empty";
    }
}

TEST_F(UsbSysfsTest, CheckProperties) {
    auto devices = usbManager.enumerateDevices();
    for (const auto& dev : devices) {
        // Basic sanity checks on what we read
        // path should contain the sysfs root (or at least be a valid string)
    }
}

TEST_F(UsbSysfsTest, SafeIoReadTest) {
    auto devices = usbManager.enumerateDevices();
    for (const auto& dev : devices) {
        // Try to read device descriptor (standard sysfs file 'descriptors')
        // This is a binary file in sysfs
        std::string descriptorPath = dev.path + "/descriptors";
        std::ifstream file(descriptorPath, std::ios::binary);
        if (file.is_open()) {
             // Read strict amount
             char buffer[18]; // Device descriptor is 18 bytes
             file.read(buffer, sizeof(buffer));
             if (file.gcount() > 0) {
                 SUCCEED() << "Successfully read " << file.gcount() << " bytes from " << descriptorPath;
             }
        }
    }
}

TEST_F(UsbSysfsTest, UsbDebugfsEnumeration) {
    std::string debugfsRoot = "/sys/kernel/debug/usb";
    const char* envPath = std::getenv("PLATFORM_TEST_USB_DEBUGFS_ROOT");
    if (envPath) {
        debugfsRoot = envPath;
    }

    if (!std::filesystem::exists(debugfsRoot)) {
        GTEST_SKIP() << "USB Debugfs root not found at " << debugfsRoot;
    }

    // specific tests for debugfs if it exists
    // e.g., checking if 'devices' file exists and is readable
    std::string devicesPath = debugfsRoot + "/devices";
    if (std::filesystem::exists(devicesPath)) {
        std::string content = utils::FsHelper::readNode(devicesPath);
        EXPECT_FALSE(content.empty()) << "USB Debugfs devices file is empty";
    }
}
