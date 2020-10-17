cmake_minimum_required(VERSION 3.12)

set(This asio)
project(${This})

find_package(OpenSSL REQUIRED)


FILE(GLOB_RECURSE HeaderFiles  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
#FILE(GLOB_RECURSE SourceFiles  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${This} INTERFACE ${HeaderFiles})
target_include_directories(${This} INTERFACE include)
