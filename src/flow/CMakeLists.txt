glob_flow_source(FLOW_SRC)

add_library(flow STATIC ${FLOW_SRC})
target_link_libraries(flow PRIVATE fdbdeps)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)