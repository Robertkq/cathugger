cmake_minimum_required(VERSION 3.2)
project(cathugger)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(dpp REQUIRED)
find_package(nlohmann_json REQUIRED)


add_executable(cathugger ${SOURCES})

target_precompile_headers(cathugger PRIVATE "include/common.h")

target_include_directories(cathugger PRIVATE "include")
target_include_directories(cathugger PRIVATE ${dpp_INCLUDE_DIRS})
target_include_directories(cathugger PRIVATE ${nlohmann_json_INCLUDE_DIRS})
target_link_libraries(cathugger dpp::dpp)
target_link_libraries(cathugger nlohmann_json::nlohmann_json)