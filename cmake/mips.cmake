############ CROSS COMPILE SETTINGS ##################

## 设置交叉编译工具链
SET(COMPILER_TOOLCHAIN_PREFIX "mips-linux-")
SET(CMAKE_C_COMPILER          ${COMPILER_TOOLCHAIN_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER        ${COMPILER_TOOLCHAIN_PREFIX}c++)	
SET(CMAKE_AR                  ${COMPILER_TOOLCHAIN_PREFIX}ar)
SET(CMAKE_RANLIB              ${COMPILER_TOOLCHAIN_PREFIX}ranlib)
SET(CMAKE_LD                  ${COMPILER_TOOLCHAIN_PREFIX}ld)

## 设置OS_NAME
SET(OS_NAME MIPS)

## Setting Compile Option
#SET(RELEASE_MODE false)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_FIND_ROOT_PATH /opt/trendchip/mips-linux-uclibc )
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#INCLUDE_DIRECTORIES(/home/maemo/mipstools/mips-linux-uclibc/usr/include)
