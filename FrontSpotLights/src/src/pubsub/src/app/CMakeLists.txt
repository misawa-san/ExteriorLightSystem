cmake_minimum_required(VERSION 3.5)
project(targetapp C)

add_library(libapp STATIC ./FrontSpotLights.c)
target_include_directories(libapp PUBLIC  ../stub)
target_compile_options(libapp PRIVATE -save-temps=obj -coverage -g)

set_target_properties(libapp
    PROPERTIES
    LINKER_LANGUAGE C
)

