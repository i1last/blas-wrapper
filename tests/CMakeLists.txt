cmake_minimum_required(VERSION 3.15)

project(blas_wrapper_tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB TEST_FILES_PATHS
    vector/*.cpp
    # matrix/*.cpp
)

foreach(TEST_FILE ${TEST_FILES_PATHS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    add_executable(${TEST_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_NAME}
        PRIVATE blas_wrapper
        PRIVATE gtest_main
    )

    target_include_directories(${TEST_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/blas_wrapper/include
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()