cmake_minimum_required(VERSION 3.0.0)
project(PixelGLFrontend VERSION 0.1.0)
#SET(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
#SET_SOURCE_FILES_PROPERTIES( main.c PROPERTIES LANGUAGE C )
#SET_SOURCE_FILES_PROPERTIES( main.c PROPERTIES LINKER_LANGUAGE C )

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW::glew)
include_directories(${GLEW_INCLUDE_DIR})

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)
include_directories(${GLFW3_INCLUDE_DIRS})
