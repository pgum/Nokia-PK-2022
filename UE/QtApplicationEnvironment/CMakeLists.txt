cmake_minimum_required(VERSION 3.12)

project(QtUeApplicationEnvironment)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UE_QTAPPENV_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(UE_GUI_DIR ${UE_QTAPPENV_DIR}/GUI)
set(UE_TRANSPORT_DIR ${UE_QTAPPENV_DIR}/Transport)

include_directories(${UE_APP_DIR})
include_directories(${UE_APPENV_DIR})

add_subdirectory(GUI)
add_subdirectory(Transport)

set_qt_options()
aux_source_directory(. SRC_LIST)
add_library(${PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${PROJECT_NAME} UeApplicationEnvironment)
target_link_libraries(${PROJECT_NAME} QtUeGUI)
target_link_libraries(${PROJECT_NAME} QtUeTransport)
qt5_use_modules(${PROJECT_NAME}  Widgets)
qt5_use_modules(${PROJECT_NAME}  Network)

target_link_qt()

