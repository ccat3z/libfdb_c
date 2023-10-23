glob_flow_source(FDBRPC_SRC)

add_library(fdbrpc STATIC ${FDBRPC_SRC})
target_link_libraries(fdbrpc PRIVATE flow)

add_library(fdbb64 STATIC libb64/cdecode.c libb64/cencode.c)
target_link_libraries(fdbrpc PRIVATE fdbb64)

add_library(fdbeio STATIC libeio/eio.c)
target_compile_definitions(fdbeio PRIVATE USE_UCONTEXT)
target_compile_options(fdbeio BEFORE PRIVATE -w) # disable warnings for eio
target_link_libraries(fdbrpc PRIVATE fdbeio)
