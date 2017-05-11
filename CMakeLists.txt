cmake_minimum_required(VERSION 3.7)
project(mesh)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/billengine.cpp
        src/billGLfunctions.cpp
        src/billmaterialpoint.cpp
        src/billwindow.cpp
        src/oscillator.cpp
        headers/plane.h
        headers/triangle.h
        headers/Icosahedron.h
        main.cpp)

#mac
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(headers)
add_executable(mesh ${SOURCE_FILES})
target_link_libraries(mesh ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# linux
#add_executable(mesh ${SOURCE_FILES})
#include_directories(headers)
#
#target_link_libraries(mesh GL GLU glut)