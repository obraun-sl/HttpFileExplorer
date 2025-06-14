cmake_minimum_required(VERSION 3.10)
project(HttpFileExplorer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(HttpFileExplorer src/main.cpp include/httplib.h)
target_link_libraries(HttpFileExplorer Qt5::Core)
