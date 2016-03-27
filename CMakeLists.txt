cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(apc-bow)

find_package( OpenCV 3 REQUIRED )

set(SRC_CPP
    src/utils.cpp)


INCLUDE_DIRECTORIES(src)

add_executable(apc-bow ${SRC_CPP}  main.cpp )
target_link_libraries(apc-bow  ${OpenCV_LIBS} )

