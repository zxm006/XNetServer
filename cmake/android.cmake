
SET(CMAKE_TOOL_ROOT_PATH /opt/android4.4/bin)
SET(CMAKE_C_COMPILER ${CMAKE_TOOL_ROOT_PATH}/arm-linux-androideabi-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_TOOL_ROOT_PATH}/arm-linux-androideabi-g++)
SET(CMAKE_AR ${CMAKE_TOOL_ROOT_PATH}/arm-linux-androideabi-ar)
SET(CMAKE_LD ${CMAKE_TOOL_ROOT_PATH}/arm-linux-androideabi-ld)
SET(CMAKE_RANLIB ${CMAKE_TOOL_ROOT_PATH}/arm-linux-androideabi-ranlib)
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie -fPIE -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16")
SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -pie -fPIE -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16")

SET(OS_NAME ANDROID)
ADD_DEFINITIONS(-D__ANDROID_OS__) 