cmake_minimum_required(VERSION 3.10)

project(Airlines)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES "src/*.cpp" "src/**/*.cpp")

add_executable(airlines ${SRC_FILES})
target_include_directories(airlines PRIVATE src)

set(ORTOOLS_PATH "${CMAKE_SOURCE_DIR}/third_party/or-tools_x86_64_Ubuntu-22.04_cpp_v9.11.4210")
target_include_directories(airlines PRIVATE ${ORTOOLS_PATH}/include)
target_link_directories(airlines PRIVATE ${ORTOOLS_PATH}/lib)

target_link_libraries(airlines ortools)
