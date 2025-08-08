cmake_minimum_required(VERSION 3.2)
project(cathugger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(dpp REQUIRED)


add_executable(cathugger ${SOURCES})
target_include_directories(cathugger PRIVATE "include")
target_include_directories(cathugger PRIVATE ${dpp_INCLUDE_DIRS})
target_link_libraries(cathugger dpp::dpp)