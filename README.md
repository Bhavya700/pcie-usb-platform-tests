# pcie-usb-platform-tests

Linux PCIe & USB platform test suite using C++ GoogleTest and Python orchestration.

## Prerequisites

- CMake (version 3.10 or higher)
- Google Test framework
- Root access (for debugfs operations)
- Linux kernel with PCIe driver support

## Building the Tests
 
1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure CMake:
```bash
cmake ..
```

3. Build the tests:
```bash
make
```

## Running the Tests

### Option 1: Python Orchestration (Recommended)

This script automates test execution along with log capturing and report generation.

```bash
python3 scripts/run_platform_tests.py --bin ./build/pcie_tests --out ./results
```

This will generate:
- `results/report.html`: A visual report of pass/fail status and logs.
- `results/test_detail.xml`: Generic JUnit XML output.

### Option 2: Manual Execution

Run the binary directly (requires root for debugfs interactions):

```bash
sudo ./build/pcie_tests
```

## Configuration

You can override the default paths using environment variables:

- `PLATFORM_TEST_PCIE_DEBUGFS_ROOT`: Path to PCIe debugfs root (Default: `/sys/kernel/debug/msm_pcie/`)
- `PLATFORM_TEST_USB_SYSFS_ROOT`: Path to USB sysfs devices (Default: `/sys/bus/usb/devices`)

## Test Structure

### PCIe Driver Tests
- **Debugfs Checks**: Validates nodes like `aer_enable`, `boot_option`, `base_sel`.
- **Status Validation**: Checks `dmesg` output after operations.

### USB Platform Tests
- **Enumeration**: Lists devices via sysfs tree.
- **Descriptors**: Reads standard USB properties (`idVendor`, `idProduct`, `speed`).
- **IO Validation**: Performs safe reads on generic attributes to verify filesystem stability.

## Troubleshooting

1. If tests fail with permission errors:
   - Ensure you're running with `sudo`.
   - Check that debugfs is mounted at `/sys/kernel/debug`.

2. "GTest cstddef not found" during build:
   - This often happens mostly on MacOS if SDK is not set up perfectly. Ensure you have the latest Command Line Tools installed.

   - On Linux/Ubuntu, install `build-essential` and `cmake`. 
