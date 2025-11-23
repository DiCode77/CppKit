cmake_minimum_required(VERSION 3.27)
project(CppKit)

set(LIST "${CMAKE_SOURCE_DIR}/list")

set(SRC "${CMAKE_SOURCE_DIR}/src")

include_directories(${PROJECT_NAME} PRIVATE ${LIST} ${SRC})

file(GLOB_RECURSE LIST_FILES "${LIST}/*.hpp" "${LIST}/*.cpp")
file(GLOB_RECURSE SRC_FILES  "${SRC}/*.hpp" "${SRC}/*.cpp")

add_executable(${PROJECT_NAME} ${LIST_FILES} ${SRC_FILES})

source_group("list" FILES ${LIST_FILES})
source_group("src"  FILES ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)