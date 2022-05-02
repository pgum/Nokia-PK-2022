project(UeApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(States SRC_LIST)
aux_source_directory(SMS SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST} SMS/SMS.cpp  SMS/SMS.hpp SMS/SMS_DB.cpp SMS/SMS_DB.hpp States/ViewingSmsState.cpp States/ViewingSmsState.hpp)
target_link_libraries(${PROJECT_NAME} Common)
