set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -Wall -Wextra -pedantic")

add_definitions(-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(cp2utf8
    cp2utf8.cc
    cp2utf8.h
    tbl_parse.cc
    tbl_parse.h
    to_utf8.cc
    to_utf8.h
)

install(TARGETS cp2utf8 RUNTIME DESTINATION bin)
