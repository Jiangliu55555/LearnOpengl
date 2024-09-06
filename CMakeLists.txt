cmake_minimum_required(VERSION 3.28)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 14) # this does nothing for MSVC, use target_compile_options below

# include
include_directories(${CMAKE_SOURCE_DIR}/includes)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# Find Lib
link_directories(${CMAKE_SOURCE_DIR}/lib)

#find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS ${CMAKE_SOURCE_DIR}/lib)
#find_library(ASSIMP_LIBRARY NAMES assimp.dll PATHS ${CMAKE_SOURCE_DIR}/lib)


set(LIBS glfw3 opengl32 assimp)

# first create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

add_executable(01PlayWindow "src/Basic/01PlayWindow/playwindow.cpp")

target_link_libraries(01PlayWindow
        ${LIBS})

add_executable(02DrawTriangle "src/Basic/02DrawTriangle/draw_triangle.cpp")

target_link_libraries(02DrawTriangle
        ${LIBS})

add_executable(03DrawQuad "src/Basic/03DrawQuad/draw_quad.cpp")

target_link_libraries(03DrawQuad
        ${LIBS})

add_executable(04Shader "src/Basic/04Shader/use_shader.cpp")

target_link_libraries(04Shader
        ${LIBS})

add_executable(05Texture "src/Basic/05Texture/draw_texture.cpp")

target_link_libraries(05Texture
        ${LIBS})

add_executable(06Transform "src/Basic/06Transform/use_transform.cpp")

target_link_libraries(06Transform
        ${LIBS})

add_executable(07MVPTransform "src/Basic/07MVPTransform/mvp_transform.cpp")

target_link_libraries(07MVPTransform
        ${LIBS})

add_executable(08Camera "src/Basic/08Camera/custom_camera.cpp")

target_link_libraries(08Camera
        ${LIBS})

add_executable(01LoadModel "src/LoadModel/01LoadModel/loadModel.cpp")

target_link_libraries(01LoadModel
        ${LIBS})

file(GLOB DLLS "dlls/*.dll")
add_custom_command(TARGET 01LoadModel PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLLS} $<TARGET_FILE_DIR:01LoadModel>)
