cmake_minimum_required(VERSION 3.5)
if (${ARCH_TAG} STREQUAL "linux_x86")
    message("---- Build for linux_x86 ----")
    set(ARCH_TAG "linux")
    set(CMAKE_STRIP "strip")
elseif (${ARCH_TAG} STREQUAL "linux_arm")
    message("---- Build for linux_arm ----")
    set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
    set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
    set(ARCH_TAG "arm")
    include_directories("/usr/local/arm-ssl/include")
    set(OPENSSL_ROOT_DIR "/usr/local/arm-ssl")
    set(CMAKE_STRIP "aarch64-linux-gnu-strip")
else()
    message(FATAL_ERROR "Unknown target platform: ${ARCH_TAG}")
endif ()

project(Demo)

set(PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/..)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_INCLUDE_DIRS})

add_executable(Demo demo.cpp)

find_package(OpenSSL REQUIRED)

target_link_libraries(Demo
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_custom_command(TARGET Demo
                    POST_BUILD
                    COMMAND ${CMAKE_STRIP} ${CMAKE_SOURCE_DIR}/build/bin/Demo -o ${CMAKE_SOURCE_DIR}/output/Demo-${ARCH_TAG}
                    WORKING_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR})