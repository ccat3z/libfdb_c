if(LINUX)
    set(os "linux")
elseif(APPLE)
    set(os "osx")
else()
    message(FATAL_ERROR "Unsupport os")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(cpu "aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(ppc64le|powerpc64le)")
    set(cpu "ppc64le")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(cpu "intel")
else()
    message(FATAL_ERROR "Unsupport processor: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(FDB_C_ASM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${os}-${cpu})

add_library(fdb_c fdb_c.cpp fdb_c.actor.g.cpp ${FDB_C_ASM_DIR}/fdb_c.g.S)
target_include_directories(fdb_c
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${FDB_C_ASM_DIR}
)
target_link_libraries(fdb_c PRIVATE fdbclient fdbdeps)
