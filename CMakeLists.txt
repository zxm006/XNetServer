cmake_minimum_required(VERSION 2.8)

project(XNetServer)

include(cmake/version.cmake)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")

endif()

include_directories(${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

FILE (GLOB ALL_SOURCES   "./src/*.cpp")
FILE (GLOB ALL_INCLUDES "./src/*.h")
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${ALL_SRCS})

SET (ALL_SRCS 
	 ${ALL_SOURCES}
	 ${ALL_INCLUDES}
)

# aux_source_directory(./src DIR_EXE_SRCS)

     set(3RD_LIBS XNet)
     set(SYS_LIBS pthread)
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -fPIC -std=c99")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -fPIC -std=c++11")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO} -fPIC -Wno-unused -std=c99")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fPIC -Wno-unused -std=c++11") 
 
    set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_NAME})
 

link_directories(
    ${VENDOR_DIR}
)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)
message(STATUS "EXECUTABLE_OUTPUT_PATH:  ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "VENDOR_DIR:  ${VENDOR_DIR}")
message(STATUS "LIBRARY_OUTPUT_PATH:     ${LIBRARY_OUTPUT_PATH}")


message(STATUS "CMAKE_C_FLAGS_DEBUG:   ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO:   ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

 add_executable(XNetServer ${ALL_SRCS} )
target_link_libraries(XNetServer ${SYS_LIBS} ${3RD_LIBS})
