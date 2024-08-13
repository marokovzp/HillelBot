cmake_minimum_required(VERSION 3.26)
project(hillelbot)

set(SRC "${CMAKE_SOURCE_DIR}/src")
message("Hello SRC DIR IS ${SRC}")
include_directories(${SRC})

set(My "My")

add_executable(${My}${PROJECT_NAME} ${SRC}/main.cpp)
set_target_properties(${My}${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                      CXX_STANDARD_REQUIRED ON)