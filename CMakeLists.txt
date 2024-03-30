cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)


project(ysn CXX)

# 20 for Codeforces, 23 for AtCoder
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
    GLOB_RECURSE TEST_CXX_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_DEPENDS test/*.cpp)

add_executable(${PROJECT_NAME} ${TEST_CXX_FILES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    GTest::gtest GTest::gtest_main)
    
target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/template)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
