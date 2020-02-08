set(TARGET cache-benchmark)

set(SRC_LIST main.cpp)

add_executable(${TARGET} ${SRC_LIST})

find_package(benchmark REQUIRED)
target_link_libraries(${TARGET} PRIVATE benchmark::benchmark)

set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
