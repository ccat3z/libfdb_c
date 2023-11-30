project(fdb_c C CXX ASM)

function(glob_flow_source VAR_NAME)
    file(GLOB SOURCE_FILES *.c *.cpp *.h)
    set(${VAR_NAME})

    foreach(SOURCE_FILE ${SOURCE_FILES})
        if(NOT SOURCE_FILE MATCHES "actor\\.h$" AND NOT SOURCE_FILE MATCHES "actor\\.cpp$")
            list(APPEND ${VAR_NAME} ${SOURCE_FILE})
        endif()
    endforeach()

    set(${VAR_NAME} "${${VAR_NAME}}" PARENT_SCOPE)
endfunction()

if(NOT TARGET fdbdeps)
    find_package(Boost REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(msgpack-cxx CONFIG REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(absl CONFIG REQUIRED)

    add_library(fdbdeps INTERFACE)
    target_link_libraries(fdbdeps INTERFACE
        Boost::headers
        fmt::fmt
        msgpack-cxx
        OpenSSL::SSL
        OpenSSL::Crypto
        absl::debugging
    )
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

# Sanitizer
if(DEFINED USE_ASAN)
    add_compile_definitions(ADDRESS_SANITIZER)
endif()

# Rename ambiguous symbols
file(STRINGS "symbols.list" SYMBOL_LISTS)
foreach(symbol IN LISTS SYMBOL_LISTS)
    add_compile_definitions(${symbol}=_fdb_${symbol})
endforeach()

add_compile_definitions(NO_INTELLISENSE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)
add_subdirectory(src/flow)
add_subdirectory(src/fdbrpc)
add_subdirectory(src/fdbclient)
add_subdirectory(src/bindings/c)

add_subdirectory(examples)
