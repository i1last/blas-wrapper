cmake_minimum_required(VERSION 3.15)

# Compiler config
set(CMAKE_C_COMPILER "/opt/intel/oneapi/compiler/2025.1/bin/icx" CACHE STRING "C Compiler" FORCE)
set(CMAKE_CXX_COMPILER "/opt/intel/oneapi/compiler/2025.1/bin/icpx" CACHE STRING "C++ Compiler" FORCE)
set(CMAKE_Fortran_COMPILER "/opt/intel/oneapi/compiler/2025.1/bin/ifx" CACHE STRING "Fortran compiler" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verbose by default
if(NOT DEFINED COMPILER_VERBOSE)
    set(COMPILER_VERBOSE OFF CACHE BOOL "-v flag to compiler")
    message(STATUS ":: Set COMPILER_VERBOSE = OFF by default")
endif()

# Testing by default
if(NOT DEFINED TESTING_MODE)
    set(TESTING_MODE OFF CACHE BOOL "Enable testing mode with debug flags")
    message(STATUS ":: Set TESTING_MODE = OFF by default")
endif()

if(COMPILER_VERBOSE)
    message(STATUS ":: :: COMPILER_VERBOSE is ON - using -v flag for compile")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
else()
    message(STATUS ":: COMPILER_VERBOSE is OFF - NO using -v flag for compile")
endif()

if(TESTING_MODE)
    message(STATUS ":: TESTING_MODE is ON - adding debug flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -debug -Rno-debug-disables-optimization")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -debug")
else()
    message(STATUS ":: TESTING_MODE is OFF - NO debug flags")
endif()

# Project declare
project(blas_wrapper_project LANGUAGES CXX)

# Using MKL tools by Intel
if(CMAKE_CXX_COMPILER_ID MATCHES "Intel" OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    message(STATUS ":: Intel C++ compiler detected (${CMAKE_CXX_COMPILER_ID}). Adding -qmkl=sequential globally to CMAKE_CXX_FLAGS.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qmkl=sequential")
else()
    message(WARNING ":: Non-Intel C++ compiler detected (${CMAKE_CXX_COMPILER_ID}). MKL flag (-qmkl=sequential) not added automatically. Manual MKL configuration might be needed.")
endif()

# Debug info
message(STATUS ":: C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS ":: C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS ":: C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS ":: Effective CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")


# Blas Wrapper CMake
add_subdirectory(blas_wrapper)

# GTest CMake
if(NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS ON CACHE BOOL "Build the tests")
    message(STATUS ":: Set BUILD_TESTS = ON by default")
endif()

if(BUILD_TESTS)
    message(STATUS ":: BUILD_TESTS is ON - including tests")
    add_subdirectory(tests)
else()
    message(STATUS ":: BUILD_TESTS is OFF - skipping tests")
endif()