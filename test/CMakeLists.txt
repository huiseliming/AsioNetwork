cmake_minimum_required(VERSION 3.12)

set(This network)
project(${This})
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
set(DependentLibraries ${DependentLibraries} glfw)

if (UNIX)
    set(DependentLibraries ${DependentLibraries} pthread dl)
endif()

target_link_libraries(${This} PRIVATE ${DependentLibraries})

#add_custom_command(TARGET ${This} POST_BUILD 
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
#    ${CMAKE_CURRENT_SOURCE_DIR}/config.json 
#    ${CMAKE_CURRENT_SOURCE_DIR}/info.json 
#    $<TARGET_FILE_DIR:${This}>)

