cmake_minimum_required(VERSION 3.0.0)
project(PixelGLFrontend CXX)
set(CMAKE_VERBOSE_MAKEFILE)

#INCLUDE(CMakeForceCompiler)
#CMAKE_FORCE_C_COMPILER(gcc GNU)
#CMAKE_FORCE_CXX_COMPILER(g++ GNU)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp)
#set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::glew)
include_directories(${GLEW_INCLUDE_DIR})

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)
include_directories(${GLFW3_INCLUDE_DIRS})
