cmake_minimum_required(VERSION 3.12)

set(This AsioNetworkTest)
project(${This})

find_package(GTest CONFIG REQUIRED)

FILE(GLOB_RECURSE SourceFiles  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    )

message(${SourceFiles})

add_executable(${This} ${SourceFiles})

target_include_directories(${This} PUBLIC include)
target_include_directories(${This} PRIVATE src)

set(DependentLibraries ${DependentLibraries} gd_framework)
set(DependentLibraries ${DependentLibraries} asio)
set(DependentLibraries ${DependentLibraries} GTest::gtest_main)

if (UNIX)
    set(DependentLibraries ${DependentLibraries} pthread dl)
endif()

target_link_libraries(${This} PRIVATE ${DependentLibraries})
add_test(NAME AsioNetworkTest COMMAND AsioNetworkTest)
#add_custom_command(TARGET ${This} POST_BUILD 
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
#    ${CMAKE_CURRENT_SOURCE_DIR}/config.json 
#    ${CMAKE_CURRENT_SOURCE_DIR}/info.json 
#    $<TARGET_FILE_DIR:${This}>)

