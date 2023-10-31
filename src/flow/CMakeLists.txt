glob_flow_source(FLOW_SRC)

add_library(flow STATIC ${FLOW_SRC})
target_link_libraries(flow
    PRIVATE
        Boost::headers
        Boost::filesystem
        fmt::fmt
        absl::debugging
        OpenSSL::SSL
        OpenSSL::Crypto
        crc32
        SimpleOpt
)
target_include_directories(flow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include)

make_directory(${CMAKE_CURRENT_BINARY_DIR}/include/flow)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/include/flow/config.h)