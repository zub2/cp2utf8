cmake_minimum_required(VERSION 2.6)
project(cp2utf8)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

add_subdirectory(src)
add_subdirectory(test)

install(DIRECTORY tables DESTINATION share/cp2utf8)
install(FILES README.md DESTINATION share/doc/cp2utf8)
