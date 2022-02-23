cmake_minimum_required(VERSION 3.12)

project(QtBtsApplicationEnvironment)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BTS_QTAPPENV_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BTS_Console_DIR ${BTS_QTAPPENV_DIR}/GUI)
set(BTS_TRANSPORT_DIR ${BTS_QTAPPENV_DIR}/Transport)

include_directories(${BTS_APP_DIR})
include_directories(${BTS_APPENV_DIR})

add_subdirectory(Console)
add_subdirectory(Transport)

set_qt_options()

aux_source_directory(. SRC_LIST)
add_library(${PROJECT_NAME} ${SRC_LIST})
qt5_use_modules(${PROJECT_NAME}  Widgets)
qt5_use_modules(${PROJECT_NAME}  Network)

target_link_libraries(${PROJECT_NAME} BtsApplicationEnvironment)
target_link_libraries(${PROJECT_NAME} QtBtsConsole)
target_link_libraries(${PROJECT_NAME} QtBtsTransport)
target_link_qt()

