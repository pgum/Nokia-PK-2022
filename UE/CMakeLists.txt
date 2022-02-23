cmake_minimum_required(VERSION 3.12)

project(UE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UE_APP_DIR ${UE_DIR}/Application)
set(UE_APPENV_DIR ${UE_DIR}/ApplicationEnvironment)
set(UE_QTAPPENV_DIR ${UE_DIR}/QtApplicationEnvironment)
set(COMMON_DIR ${UE_DIR}/../COMMON)

include_directories(${UE_APP_DIR})
include_directories(${UE_APPENV_DIR})
include_directories(${COMMON_DIR})

add_subdirectory(Application)
add_subdirectory(ApplicationEnvironment)
add_subdirectory(QtApplicationEnvironment)
add_subdirectory(Tests)

set_qt_options()
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${PROJECT_NAME} Common)
target_link_libraries(${PROJECT_NAME} UeApplication)
target_link_libraries(${PROJECT_NAME} UeApplicationEnvironment)
target_link_libraries(${PROJECT_NAME} QtUeApplicationEnvironment)
target_link_libraries(${PROJECT_NAME} QtUeGUI)
target_link_libraries(${PROJECT_NAME} QtUeTransport)
qt5_use_modules(${PROJECT_NAME}  Widgets)
qt5_use_modules(${PROJECT_NAME}  Network)
target_link_qt()

set(IMAGES_DIRECTORY ${UE_QTAPPENV_DIR}/GUI)
copy_images()
