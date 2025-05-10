cmake_minimum_required(VERSION 3.15)

project(blas_wrapper LANGUAGES CXX)

add_library(blas_wrapper INTERFACE)
target_include_directories(blas_wrapper
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE blas_wrapper)