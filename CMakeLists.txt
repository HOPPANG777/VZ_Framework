cmake_minimum_required(VERSION 3.16)
project(VZ_Framework)

# Use vcpkg automatically if available
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Framework)
add_subdirectory(Script)
add_subdirectory(Client)
add_subdirectory(Tools)