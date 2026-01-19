cmake_minimum_required(VERSION 3.10)
project(PCIe_Driver_Tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}/googlemock/include)

# Add source files
add_executable(pcie_tests
    src/main.cpp
    src/pcie_test.cpp
    src/debugfs_handler.cpp
    src/utils/fs_helper.cpp
    src/usb/usb_manager.cpp
    src/usb_test.cpp
)

# Link against GTest
target_link_libraries(pcie_tests
    gtest_main
    gtest
    pthread
) 