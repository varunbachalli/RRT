cmake_minimum_required(VERSION 3.15)
project(RRT)
find_package(SDL2 REQUIRED)
include_directories(RRT include)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(RRT src/main.cpp src/printpaths.cpp src/RRT.cpp) 
target_link_libraries(RRT ${SDL2_LIBRARIES})