# Miner cross compiling toolchain file.

set(CMAKE_HOST_SYSTEM_NAME       Linux)
set(CMAKE_HOST_SYSTEM_VERSION    3.10.33)
set(CMAKE_HOST_SYSTEM_PROCESSOR  armv7l)

set(CMAKE_C_COMPILER    arm-openwrt-linux-gcc)
set(CMAKE_CXX_COMPILER  arm-openwrt-linux-c++)

set(CMAKE_FIND_ROOT_PATH /opt/staging_dir/target-arm_cortex-a5+neon_eglibc-2.19_eabi)
set(CMAKE_SYSROOT        /opt/staging_dir/target-arm_cortex-a5+neon_eglibc-2.19_eabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
