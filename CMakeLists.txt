
cmake_minimum_required(VERSION 3.23)
project(thick-pointers)

add_library(
    thick-pointers
    INTERFACE
    )


target_compile_features(
    thick-pointers
    INTERFACE
    cxx_std_17
    )

target_include_directories(
    thick-pointers
    INTERFACE
    include
    )


add_executable(
    thick-pointers-test1
    test/test1.cpp
    )

target_link_libraries(
    thick-pointers-test1
    thick-pointers
    )
