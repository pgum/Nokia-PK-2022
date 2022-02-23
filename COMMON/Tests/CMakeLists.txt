project(COMMON_UT)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_gtest_options()

include_directories(${COMMON_DIR})
aux_source_directory(. TEST_SRC_LIST)
add_subdirectory(Mocks)

add_executable(${PROJECT_NAME} ${TEST_SRC_LIST})
target_link_libraries(${PROJECT_NAME} CommonUtMocks)
target_link_libraries(${PROJECT_NAME} Common)
target_link_gtest()



