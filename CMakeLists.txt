cmake_minimum_required(VERSION 3.27)
project(CppKit)

set(LIST "${CMAKE_SOURCE_DIR}/list")
set(STRG "${CMAKE_SOURCE_DIR}/string")
set(UNIR "${CMAKE_SOURCE_DIR}/unique_ptr")

set(SRC "${CMAKE_SOURCE_DIR}/src")

include_directories(${PROJECT_NAME} PRIVATE ${LIST} ${STRG} ${UNIR} ${SRC})

file(GLOB_RECURSE LIST_FILES "${LIST}/*.hpp" "${LIST}/*.cpp")
file(GLOB_RECURSE STRG_FILES "${STRG}/*.hpp" "${STRG}/*.cpp")
file(GLOB_RECURSE UNIR_FILES "${UNIR}/*.hpp" "${UNIR}/*.cpp")
file(GLOB_RECURSE SRC_FILES  "${SRC}/*.hpp" "${SRC}/*.cpp")

add_executable(${PROJECT_NAME} ${LIST_FILES} ${STRG_FILES} ${UNIR_FILES} ${SRC_FILES})

source_group("list" FILES ${LIST_FILES})
source_group("string" FILES ${STRG_FILES})
source_group("unique_ptr" FILES ${UNIR_FILES})
source_group("src"  FILES ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)