cmake_minimum_required(VERSION 3.0.0)
project(PixelGLFrontend CXX)

set(CMAKE_VERBOSE_MAKEFILE)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

# for debugging
add_definitions(-DPIXELGL_DEBUG)

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

# PNG dependency
find_package(ZLIB REQUIRED)
link_libraries(ZLIB::ZLIB)
include_directories(${ZLIB_DEPS})

# PNG++ Dependency
find_package(PNG REQUIRED)
link_libraries(PNG::PNG)
include_directories(${PNG_INCLUDE_DIRS})

# Header only, nothing to link
find_package(png++ REQUIRED)
include_directories(${png++_INCLUDE_DIR})

add_executable(GameOfLife "${PROJECT_SOURCE_DIR}/Projects/ConwayGameOfLife/main.cpp")
add_executable(ParticleSim "${PROJECT_SOURCE_DIR}/Projects/ParticleSim/main.cpp")
add_executable(${PROJECT_NAME} main.cpp)

add_custom_command(TARGET PixelGLFrontend POST_BUILD
    COMMAND cd ${PROJECT_SOURCE_DIR} && rm -rf "./build/assets" && ln -s ../assets/ ./build/assets
)