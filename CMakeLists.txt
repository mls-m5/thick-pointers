
cmake_minimum_required(VERSION 3.23)
project(thick-pointers)

add_executable(
    thick-pointers
    src/main.cpp
    )

target_compile_features(
    thick-pointers
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    thick-pointers
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
