project(QtUeTransport)
set_qt_options()

cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. SRC_LIST)
add_library(${PROJECT_NAME} ${SRC_LIST})
qt5_use_modules(${PROJECT_NAME}  Widgets)
qt5_use_modules(${PROJECT_NAME}  Network)

target_link_qt()
