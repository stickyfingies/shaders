cmake_minimum_required(VERSION 3.6)

project(Shaders)
cmake_policy(SET CMP0072 NEW)

# Dependencies
include(${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(glfw3 CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# App
add_executable(Shaders
    src/main.cpp
)
# Headers
target_include_directories(Shaders PRIVATE
    OpenGL::GL
    glfw
    unofficial::gl3w::gl3w
)
# Libraries
target_link_libraries(Shaders PRIVATE
    OpenGL::GL
    glfw
    unofficial::gl3w::gl3w
)