add_executable(cp2utf8_test test.cc)
target_link_libraries(cp2utf8_test cp2utf8)

add_test(cp2utf8 cp2utf8_test ${CMAKE_SOURCE_DIR}/tables)
