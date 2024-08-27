set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

cmake_minimum_required(VERSION 3.26)
project(hillelbot)

set(SRC "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCES "${SRC}/*.cxx")
file(GLOB_RECURSE HEADERS "${SRC}/*.hxx")

message("Hello SRC DIR IS ${SRC}")
include_directories(${SRC})

find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(fmt REQUIRED)

set(My "My")

add_executable(${My}${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${My}${PROJECT_NAME} PROPERTIES CXX_STANDARD 20
                                                      CXX_STANDARD_REQUIRED ON)
target_link_libraries(${My}${PROJECT_NAME} PRIVATE 
                        nlohmann_json::nlohmann_json 
                        cpr::cpr 
                        fmt::fmt)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${My}${PROJECT_NAME})