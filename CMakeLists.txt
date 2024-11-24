cmake_minimum_required(VERSION 3.12)
project(MatrixEval)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Select build type: Release or Debug" FORCE)
endif()

find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_PREFIX_PATH "/home/arso/.local/lib/python3.12/site-packages/pybind11/share/cmake/pybind11" ${CMAKE_PREFIX_PATH})

find_package(pybind11 REQUIRED)

set(SOURCES
        MatrixEvaluator.cpp
)

add_library(MatrixEval MODULE ${SOURCES})

set_target_properties(MatrixEval PROPERTIES
        PREFIX ""                          # Remove "lib" prefix
        SUFFIX ".so"                       # Set file extension to .so
)

target_link_libraries(MatrixEval PRIVATE pybind11::module Python3::Python)

target_include_directories(MatrixEval PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Building in Release mode")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffunction-sections -fdata-sections -DNDEBUG" CACHE STRING "" FORCE)
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-Wl,--gc-sections -Wl,--strip-all" CACHE STRING "" FORCE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Building in Debug mode")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "" FORCE)
        add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)
endif()
