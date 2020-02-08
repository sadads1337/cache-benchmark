cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(cache-benchmark
        VERSION 1.0
        DESCRIPTION "Tiny CPU cache benchmark"
        )

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(common)

add_subdirectory(src)
