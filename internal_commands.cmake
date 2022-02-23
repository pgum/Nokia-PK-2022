macro(set_compiler_options)
# set(CMAKE_CXX_FLAGS "-std=c++17 -DGTEST_HAS_PTHREAD=1 -g -Og ${CMAKE_CXX_FLAGS}")
# add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Og ${CMAKE_CXX_FLAGS}")
endmacro()

macro(set_qt_options)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDES})
add_definitions(${Qt5Network_DEFINITIONS})
endmacro()

macro(target_link_qt)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} pthread)
endmacro()

macro(set_gtest_options)
set(GMOCK_DIR ${CMAKE_SOURCE_DIR}/googletest/googletest)
set(GTEST_DIR ${CMAKE_SOURCE_DIR}/googletest/googlemock)

include_directories(${GMOCK_DIR}/include)
include_directories(${GTEST_DIR}/include)
include_directories(${GMOCK_DIR})
include_directories(${GTEST_DIR})
endmacro()

macro(target_link_gtest)
target_link_libraries(${PROJECT_NAME} gmock_main)
endmacro()

macro(copy_images)
file(COPY ${IMAGES_DIRECTORY}/images DESTINATION .)
endmacro()


