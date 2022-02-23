project(BtsApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(UeConnection SRC_LIST)
aux_source_directory(UeRelay SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Common)
