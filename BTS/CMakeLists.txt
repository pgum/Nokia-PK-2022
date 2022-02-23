cmake_minimum_required(VERSION 3.12)
project(BTS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_DIR ${BTS_DIR}/../COMMON)
set(BTS_APPLICATION_DIR ${BTS_DIR}/Application)
set(BTS_APPLICATION_ENVIRONMENT_DIR ${BTS_DIR}/ApplicationEnvironment)

include_directories(${COMMON_DIR})
include_directories(${BTS_APPLICATION_ENVIRONMENT_DIR})
include_directories(${BTS_APPLICATION_DIR})

add_subdirectory(Application)
add_subdirectory(ApplicationEnvironment)
add_subdirectory(QtApplicationEnvironment)
add_subdirectory(Tests)


set_qt_options()

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
qt5_use_modules(${PROJECT_NAME}  Widgets)
qt5_use_modules(${PROJECT_NAME}  Network)

target_link_libraries(${PROJECT_NAME} BtsApplication)
target_link_libraries(${PROJECT_NAME} BtsApplicationEnvironment)
target_link_libraries(${PROJECT_NAME} QtBtsApplicationEnvironment)

target_link_qt()
