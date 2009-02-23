cmake_minimum_required(VERSION 2.6)
project(cp2utf8)

add_subdirectory(src)

install(DIRECTORY tables DESTINATION share/cp2utf8)
