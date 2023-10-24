add_compile_definitions(FDB_API_VERSION=710)
add_executable(basic_rw basic_rw.cpp)
target_link_libraries(basic_rw PRIVATE fdb_c atomic)
