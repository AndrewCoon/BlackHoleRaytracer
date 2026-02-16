cmake_minimum_required(VERSION 3.15)
project(BlackHoleTracer)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(BlackHoleTracer/Vendor/glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(BlackHoleTracer/Headers/
                    BlackHoleTracer/Vendor/glad/include/
                    BlackHoleTracer/Vendor/glfw/include/
                    BlackHoleTracer/Vendor/glm/
                    BlackHoleTracer/Vendor/imgui/
                    BlackHoleTracer/Vendor/imgui/backends/)

file(GLOB VENDORS_SOURCES BlackHoleTracer/Vendor/glad/src/glad.c
                          BlackHoleTracer/Vendor/imgui/imgui.cpp
                          BlackHoleTracer/Vendor/imgui/imgui_tables.cpp
                          BlackHoleTracer/Vendor/imgui/imgui_demo.cpp
                          BlackHoleTracer/Vendor/imgui/imgui_draw.cpp
                          BlackHoleTracer/Vendor/imgui/imgui_widgets.cpp

                          BlackHoleTracer/Vendor/imgui/backends/imgui_impl_glfw.cpp
                          BlackHoleTracer/Vendor/imgui/backends/imgui_impl_opengl3.cpp)
                          
file(GLOB PROJECT_HEADERS BlackHoleTracer/Headers/*.hpp)
file(GLOB PROJECT_SOURCES BlackHoleTracer/Sources/*.cpp)
file(GLOB PROJECT_SHADERS BlackHoleTracer/Shaders/*.comp
                          BlackHoleTracer/Shaders/*.frag
                          BlackHoleTracer/Shaders/*.geom
                          BlackHoleTracer/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                          TODO.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})


if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/BlackHoleTracer/Assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif() 

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/BlackHoleTracer/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
