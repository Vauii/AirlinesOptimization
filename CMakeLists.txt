cmake_minimum_required(VERSION 3.10)

project(Airlines)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES "src/*.cpp" "src/**/*.cpp")

add_executable(airlines ${SRC_FILES})
target_include_directories(airlines PRIVATE src)
