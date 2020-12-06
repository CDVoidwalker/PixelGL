cmake_minimum_required(VERSION 3.0.0)
project(PixelGLFrontend CXX)
set(CMAKE_VERBOSE_MAKEFILE)

include_directories(${PROJECT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
link_libraries(OpenGL::GL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
link_libraries(GLEW::glew)
include_directories(${GLEW_INCLUDE_DIR})

find_package(glfw3 REQUIRED)
link_libraries(glfw)
include_directories(${GLFW3_INCLUDE_DIRS})

add_executable(GameOfLife "${PROJECT_SOURCE_DIR}/Projects/ConwayGameOfLife/main.cpp")
add_executable(ParticleSim "${PROJECT_SOURCE_DIR}/Projects/ParticleSim/main.cpp")
add_executable(${PROJECT_NAME} main.cpp)