cmake_minimum_required (VERSION 2.8)

project(liquidfiles)

find_package(CURL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src/
                    ${CURL_INCLUDE_DIRS}
                    )
SET(CMAKE_BUILD_TYPE Release)

add_definitions(-std=c++11)

SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src/)

file(GLOB SRCS_PATH ${SRC_PATH}*/*.cpp)

SET(EXECS ${SRC_PATH}/main.cpp)

add_library(liquidfiles_lib ${SRCS_PATH})

target_link_libraries(liquidfiles_lib ${CURL_LIBRARIES})

add_executable(liquidfiles ${EXECS})
target_link_libraries(liquidfiles liquidfiles_lib ${CURL_LIBRARIES})
