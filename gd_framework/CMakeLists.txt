cmake_minimum_required(VERSION 3.10)

set(This gd_framework)

#添加
file(GLOB_RECURSE GDFrameworkSourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE GDFrameworkHeaderFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

#message(${VulkanRendererHeaderFiles} )

add_library(${This} STATIC  ${GDFrameworkSourceFiles} ${GDFrameworkHeaderFiles})

target_include_directories(${This} PUBLIC include)
target_include_directories(${This} PRIVATE src)

message(${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${This} PUBLIC glfw)

#SET_TARGET_PROPERTIES(vulkan_renderer PROPERTIES VERSION 1.0 SOVERSION 1)

INSTALL(TARGETS ${This} ARCHIVE DESTINATION lib)
INSTALL(FILES GDFrameworkHeaderFiles DESTINATION include/${This})

