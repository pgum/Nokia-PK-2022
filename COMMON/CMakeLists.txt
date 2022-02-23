project(Common)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(. SRC_LIST)
aux_source_directory(Messages SRC_LIST)
aux_source_directory(Logger SRC_LIST)
aux_source_directory(Config SRC_LIST)
aux_source_directory(Traits SRC_LIST)
aux_source_directory(CommonEnvironment SRC_LIST)
aux_source_directory(TestCommands SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST})

add_subdirectory(Tests)
