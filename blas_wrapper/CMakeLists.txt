cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER icx CACHE STRING "C Compiler" FORCE)
set(CMAKE_CXX_COMPILER icpx CACHE STRING "C++ Compiler" FORCE)
set(CMAKE_Fortran_COMPILER ifx CACHE STRING "Fortran compiler" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(blas_wrapper LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "Intel" OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    message(STATUS ":: Intel C++ compiler detected (${CMAKE_CXX_COMPILER_ID}). Adding -qmkl=sequential globally to CMAKE_CXX_FLAGS.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qmkl=sequential")
else()
    message(WARNING ":: Non-Intel C++ compiler detected (${CMAKE_CXX_COMPILER_ID}). MKL flag (-qmkl=sequential) not added automatically. Manual MKL configuration might be needed.")
endif()

add_library(blas_wrapper INTERFACE)
target_include_directories(blas_wrapper
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE blas_wrapper)

message(STATUS ":: C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS ":: C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS ":: C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS ":: Effective CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}") # Посмотрим на итоговые флаги