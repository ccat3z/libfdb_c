project(fdb_c C CXX ASM)
cmake_minimum_required(VERSION 3.27)

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

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(fmt CONFIG REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(NO_INTELLISENSE)

add_subdirectory(src/contrib/SimpleOpt)
add_subdirectory(src/contrib/crc32)
add_subdirectory(src/contrib/libb64)
add_subdirectory(src/flow)
add_subdirectory(src/fdbrpc)
add_subdirectory(src/fdbclient)
add_subdirectory(src/bindings/c)

add_subdirectory(examples)
