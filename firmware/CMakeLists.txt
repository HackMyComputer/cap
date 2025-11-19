# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(COMPONENT_REQUIRES arduino)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers"
    "${CMAKE_CURRENT_LIST_DIR}/components/protocols"
    "${CMAKE_CURRENT_LIST_DIR}/components/model"
    "${CMAKE_CURRENT_LIST_DIR}/components/gateway"
    "${CMAKE_CURRENT_LIST_DIR}/components/audio" 
    "${CMAKE_CURRENT_LIST_DIR}/components/utility"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(app_main)


