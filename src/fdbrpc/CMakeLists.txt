glob_flow_source(FDBRPC_SRC)

add_library(fdbrpc STATIC ${FDBRPC_SRC})
target_link_libraries(fdbrpc
    PRIVATE
        flow
        rapidjson
        crc32
        libb64
)
target_include_directories(fdbrpc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(fdbeio STATIC libeio/eio.c)
target_include_directories(fdbeio PUBLIC libeio/)
target_compile_definitions(fdbeio PRIVATE USE_UCONTEXT)
target_compile_options(fdbeio BEFORE PRIVATE -w) # disable warnings for eio
target_link_libraries(fdbrpc PRIVATE fdbeio)
