cmake_minimum_required(VERSION 3.7)
project(box)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/billengine.cpp
        src/billGLfunctions.cpp
        src/billmaterialpoint.cpp
        src/billwindow.cpp
        src/oscillator.cpp
        src/box.cpp
        src/billRBengine.cpp
        src/billrigidbody.cpp
        main.cpp headers/AABB.h)

#mac
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(headers)
add_executable(box ${SOURCE_FILES})
target_link_libraries(box ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# linux
#add_executable(mesh ${SOURCE_FILES})
#include_directories(headers)
#
#target_link_libraries(mesh GL GLU glut)