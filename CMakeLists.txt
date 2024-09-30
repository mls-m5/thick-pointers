
cmake_minimum_required(VERSION 3.23)
project(thick-pointers)

enable_testing()

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


function(test testname)
    add_executable(
        ${testname}-test
        test/${testname}-test.cpp
        )

    target_link_libraries(
        ${testname}-test
        thick-pointers
        )

    add_test(NAME ${testname}-test
        COMMAND ${testname}-test
        )
endfunction()

test(basic)
test(constructor)
test(owning)
