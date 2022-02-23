project(CommonUtMocks)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_gtest_options()

include_directories(${COMMON_DIR})

aux_source_directory(. MOCKS_SRC_LIST)

add_library(${PROJECT_NAME} ${MOCKS_SRC_LIST})
target_link_libraries(${PROJECT_NAME} Common)
target_link_gtest()


