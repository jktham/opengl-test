cmake_minimum_required(VERSION 3.0.0)
project(opengl)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE BASICS_SOURCES basics/src/*.cpp)
file(GLOB_RECURSE LIGHTING_SOURCES lighting/src/*.cpp)
file(GLOB_RECURSE SCENES_SOURCES scenes/src/*.cpp)
file(GLOB_RECURSE EXTERNAL_SOURCES external/src/*.cpp external/src/*.c)

add_executable(basics ${BASICS_SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(basics PRIVATE external/include)
target_link_directories(basics PRIVATE external/lib/GLFW)
target_link_libraries(basics PRIVATE libglfw3.a)

add_executable(lighting ${LIGHTING_SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(lighting PRIVATE external/include)
target_link_directories(lighting PRIVATE external/lib/GLFW)
target_link_libraries(lighting PRIVATE libglfw3.a)

add_executable(scenes ${SCENES_SOURCES} ${EXTERNAL_SOURCES})
target_include_directories(scenes PRIVATE external/include)
target_link_directories(scenes PRIVATE external/lib/GLFW)
target_link_libraries(scenes PRIVATE libglfw3.a)
