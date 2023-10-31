glob_flow_source(FDBCLIENT_SRC)

list(APPEND FDBCLIENT_SRC sha1/SHA1.cpp)

add_library(fdbclient STATIC ${FDBCLIENT_SRC})
target_include_directories(fdbclient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(fdbclient
    PRIVATE
        fdbrpc
        flow
        msgpack-cxx
)
