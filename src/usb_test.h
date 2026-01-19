#pragma once

#include <gtest/gtest.h>
#include "usb/usb_manager.h"

class UsbSysfsTest : public ::testing::Test {
protected:
    usb::UsbManager usbManager;

    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Teardown code if needed
    }
};
