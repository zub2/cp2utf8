add_definitions(-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_library(cp2utf8 SHARED
    converter.cc
    converter.h
    translation_table.cc
    translation_table.h
    to_utf8.cc
    to_utf8.h
)
set_target_properties(cp2utf8 PROPERTIES VERSION 1.0 SOVERSION 1)


add_executable(cp2utf8_tool
    cp2utf8.cc
    cp2utf8.h
)
set_target_properties(cp2utf8_tool PROPERTIES OUTPUT_NAME cp2utf8)
target_link_libraries(cp2utf8_tool cp2utf8)

install(TARGETS cp2utf8 cp2utf8_tool RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(FILES converter.h DESTINATION include/cp2utf8)
